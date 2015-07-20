#include "config.h"
#include "communication.h"
#include "uart_driver.h"
#include "timer_driver.h"

#ifdef __LOOP_BACK__
#include "lcd_driver.h"
#endif

enum
{
	IGNORE = 0,
	PARSE_SUCCESS = 1,
	PARSE_FAILURE = 2
};

enum
{
	TIMEOUT = 5
};

typedef struct _COMMUNICATTION
{
	
	UINT8 state;
	UINT8 rx_sop;
	UINT8 rx_eop;
	UINT8 tx_sop;
	UINT8 tx_eop;
	UINT8 rxPacketBuffer[RX_PACKET_SIZE];
	UINT8 txPacketBuffer[TX_PACKET_SIZE];
	UINT8 rxPacketIndex;
	UINT8 txPacketLength;
	UINT8 txCode;
	UINT8 timeout;
	UINT8 (*callBack)(UINT8* cmd_data, UINT8* respID,  UINT8** resp_data);

	UINT32 prevAppTime, curAppTime;
	UINT8 prevState ;
}COMMUNICATION;

#pragma idata com_data
COMMUNICATION communication = {0};
#pragma idata

UINT16 comTimeout = 0xFFFF;
UINT8 rom alert[]={"COM\n"};


UINT8 COM_BCC( UINT8* data  , UINT8 length);
UINT8 checksum();
UINT8 parsePacket(UINT8 *respCode);
void COM_reset(void);
void COM_txData(void);
UINT8 checksum(UINT8 *buffer, UINT8 length);

void COM_init(UINT8 rx_sop , UINT8 rx_eop ,UINT8 tx_sop , UINT8 tx_eop , UINT8 (*callBack)(UINT8* rxdata, UINT8* txCode,UINT8** txPacket))
{
	
	UART_init();	//initialize uart
	

	communication.rx_sop = rx_sop;
	communication.rx_eop = rx_eop;
	communication.tx_sop = tx_sop;
	communication.tx_eop = tx_eop;
	communication.callBack = callBack;

	COM_reset();
}

void COM_reset()
{
		UART_init();
	communication.rxPacketIndex = 0;
	communication.txPacketLength = 0;
	communication.state = COM_START;
	communication.txCode = IGNORE;
	communication.timeout = TIMEOUT;
	communication.prevAppTime = communication.curAppTime;
	communication.prevState = communication.state;
}

#ifdef __LOOP_BACK__
void COM_task()
{
	UINT8 uartData = 0;


	
	if( UART_hasData() )
	{
		uartData = UART_read();	

		UART_write(uartData);
		UART_transmit();
		putCharToLcd(uartData);

		return;

	}
	--comTimeout;
	if( comTimeout == 0 )
	{
/*
		rom UINT8* ptr= alert;
		comTimeout = 0xFFFF;
		
		while(*ptr)
		{
			UART_write(*ptr);
			ptr++;
		}
		UART_transmit();
*/
		UART_init();
		//ENABLE_UART_RX_INTERRUPT();
	}

}

#else


void COM_task()
{
	UINT8 uartData = 0,i;
	communication.curAppTime = GetAppTime();
	if( communication.prevAppTime != communication.curAppTime)
	{
		if( communication.prevState == communication.state )
		{
			--communication.timeout ;
			if( communication.timeout == 0)
			{
				COM_reset();
				return;
			}
			
		}
		
		communication.prevAppTime = communication.curAppTime;
	}

	switch( communication.state)
	{
		case COM_START:

			if( UART_hasData() )
			{
				uartData = UART_read();	

			}

			if( uartData == communication.rx_sop )
			{
				communication.rxPacketIndex = 0;
				communication.state = COM_CHECK_ADDRESS;
			}
		break;

		case COM_CHECK_ADDRESS:
			if( UART_hasData()==FALSE )
				return;
			
			uartData = UART_read();	
			
			if( uartData != DEVICE_ADDRESS)
			{
				COM_reset();
				return;
			}
			communication.rxPacketBuffer[communication.rxPacketIndex++]=uartData;
			communication.state = COM_IN_PACKET_COLLECTION;
			break;

		case COM_IN_PACKET_COLLECTION:

			if( UART_hasData()==FALSE )
				return;
			
			uartData = UART_read();	

			

			if(uartData == communication.rx_eop )
			{
				UINT8 parseResult = 0;
				COM_RESP_CODE txCode = COM_RESP_NONE;
				UINT8 *txData ;
				

				parseResult = parsePacket(&txCode);		//parse packet 

				switch( parseResult)
				{
					case IGNORE:
					COM_reset();	
					return;
					
					case PARSE_SUCCESS:
											
					if( communication.callBack != 0 )
					{
						communication.txPacketLength = communication.callBack(&communication.rxPacketBuffer[COM_RX_DATA_START_INDEX], &communication.txCode,
													  &txData);
					
						communication.txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
						++communication.txPacketLength;

						communication.txPacketBuffer[COM_TX_CODE_INDEX] = communication.txCode;	//store tx code
						++communication.txPacketLength;

						for( i = COM_TX_DATA_START_INDEX ; i < communication.txPacketLength ; i++)	//store data
						{
							communication.txPacketBuffer[i] = *txData;
							txData++;
						}

					}

					else
					{
						COM_reset();
						return;
					}

					break;
					
					case PARSE_FAILURE:
					{

						communication.txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
						++communication.txPacketLength;
						
						communication.txPacketBuffer[COM_TX_CODE_INDEX] = communication.txCode;		//store tx code
						++communication.txPacketLength;
						
					}
					
					break;
					
					default:
					break;
				}
				communication.state = COM_IN_TX_DATA;
			}
			else
			{
				communication.rxPacketBuffer[communication.rxPacketIndex++]=uartData;
				if( communication.rxPacketIndex >= RX_PACKET_SIZE)
				{
					communication.txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
					++communication.txPacketLength;

					communication.txPacketBuffer[COM_TX_CODE_INDEX] = COM_RESP_OVERRUN;		//store tx code
					++communication.txPacketLength;
					
					communication.state = COM_IN_TX_DATA;
					
				}
			}
			break;

		case COM_IN_TX_DATA:

			COM_txData();

			COM_reset();
	
		break;

		default:
			COM_reset();
		break;

	}
	communication.prevState = communication.state;

}

#endif

UINT8 parsePacket(UINT8 *respCode)
{
	UINT8 receivedChecksum = communication.rxPacketBuffer[communication.rxPacketIndex-1];
	UINT8 genChecksum = 0;


	if((communication.rxPacketBuffer[COM_DEVICE_ADDRESS_INDEX] != DEVICE_ADDRESS)
			&& (communication.rxPacketBuffer[COM_DEVICE_ADDRESS_INDEX] != BROADCAST_ADDRESS) )
		return IGNORE;
	
	genChecksum = checksum(communication.rxPacketBuffer,communication.rxPacketIndex-1);
	
	if( receivedChecksum == genChecksum)
	{
		--communication.rxPacketIndex;
		communication.rxPacketBuffer[communication.rxPacketIndex] = '\0'; //remove checksum from packet
	 
		return SUCCESS;
	}
	else
	{	
		*respCode = COM_RESP_CHECKSUM_ERROR;
	 	return FAILURE;
	}
}

void COM_txPacket(UINT8*packet, UINT8 length,UINT8 type)
{
	UINT8 bcc = 0;
	UINT8 i = 0;
	
	bcc = checksum(packet, length);

	if( type == COM_CMD)
	{
		UART_write(communication.rx_sop);
	}
	else UART_write(communication.tx_sop);
	
	for( i = 0; i < length; i++ )
	{
		UART_write(packet[i]);
	}
	
	UART_write(bcc);

	if( type == COM_CMD)
	{
		UART_write(communication.rx_eop);
	}
	else UART_write(communication.tx_eop);

	UART_transmit();
	ClrWdt();
}	
void COM_txData()
{
	UINT8 bcc = 0;
	UINT8 i= 0;

	bcc = checksum(communication.txPacketBuffer, communication.txPacketLength);
	
	UART_write(communication.tx_sop);

	for( i = 0; i < communication.txPacketLength; i++ )
	{
		UART_write(communication.txPacketBuffer[i]);
	}

	UART_write(bcc);
	UART_write(communication.tx_eop);
#ifdef __RESPONSE_ENABLED__
	UART_transmit();
#endif
	ClrWdt();
	
}


void COM_txStr(rom UINT8 *str)
{
	while(*str)
	{
	UART_write(*str);
	*str++;
	}
	UART_transmit();
}


UINT8 checksum(UINT8 *buffer, UINT8 length)
{
	
	UINT8 bcc = 0;
	UINT8 i , j ;
	
#ifdef __BCC_LRC__

	for( i = 0 ; i < length ; i++)
	{
		bcc += buffer[i];
	}
	return bcc;

#elif defined __BCC_XOR__

	for( i = 0; i < length; i++)
	{
		bcc ^=buffer[i];
	}
	return bcc;

#endif
}		