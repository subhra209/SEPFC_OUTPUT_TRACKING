/*
*------------------------------------------------------------------------------
* pc_com_interface.c
*
* Pc commnunication interface module
*
* (C)2010 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: pc_com_interface.c
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 22/04/2010
*------------------------------------------------------------------------------
*
* Revision 0.0 22/04/2010 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <usart.h>
#include <string.h>
#include "typedefs.h"
#include "ias.h"
#include "device.h"
#include "uart_driver.h"
#include "pc_com_interface.h"
#include "lcd_driver.h"


/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/
//#define __LOOP_BACK__
#ifdef __LOOP_BACK__
//#include "lcd_driver.h"
#endif


/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/

UINT8 MyDevID = DEVICE_ID;

/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

enum RECEIVER_STATE
{
	UART_RX_STATE_IDLE,
	UART_RX_STATE_RECV_START,
	UART_RX_STATE_WAIT
};



PC_LINK_COMMAND stLinkData = {{SOF, 0x00,},0,3,UART_RX_STATE_IDLE};

static PC_LINK_COMMAND_STRUCT stLinkComm;
/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

static UINT8 validateReceivedCommand(PC_LINK_COMMAND *pstLinkData);
static void extractPacketData(PC_LINK_COMMAND *pstLinkData);
//static void displayExtractPacketData(PC_LINK_COMMAND_STRUCT *pstExtData);

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/
/*
*------------------------------------------------------------------------------
* Debug rotinues
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void displayExtractPacketData(PC_LINK_COMMAND_STRUCT *pstExtData)
*
* Summary	: Display extracted data received from the pc link
*
* Input		: PC_LINK_COMMAND_STRUCT *pstExtData - pointer to the extracted data
*
* Output	: None 
*------------------------------------------------------------------------------
*/
void displayExtractPacketData(PC_LINK_COMMAND_STRUCT *pstExtData)
{
	UINT8 index;
	PutcUARTRam(pstExtData->mDevId);
	PutcUARTRam(pstExtData->mCommand);
	PutcUARTRam(pstExtData->mSize);
	for(index = 0;index <pstExtData->mSize;index++)
	{
		PutcUARTRam(pstExtData->mData[index]);
	}
}
/*
*------------------------------------------------------------------------------
* UINT8 validateReceivedCommand(PC_LINK_COMMAND *pstLinkData)
*
* Summary	: Check the received data packet form the pc 
*
* Input		: PC_LINK_COMMAND *pstLinkDta - pointer to the received pc link data
*
* Output	: UINT8 - error code 
*------------------------------------------------------------------------------
*/
UINT8 validateReceivedCommand(PC_LINK_COMMAND *pstLinkData)
{
	INT16 bcc = 0;

	if(pstLinkData->mLeftBytes > 0)
	{
#ifdef SER_DEBUG
		PutrsUART("VALIDATE_CMD - Data Invalid!\r\n");
#endif
		return INVALID_DATA;
	}
	else 
	if(pstLinkData->mReceivedindex == 0)
	{
#ifdef SER_DEBUG
		PutrsUART("VALIDATE_CMD - Wait Reader response Time out!\r\n");
#endif
		return COMMAND_RECV_TIMEOUT;
	}
	if(pstLinkData->mPacketBuf[pstLinkData->mReceivedindex -1] != 0xBB)
	{
#ifdef SER_DEBUG
		PutrsUART("VALIDATE_CMD - Invalid data trailer\r\n");
#endif
		return INVALID_TRAILER;
	}
#ifdef COMM_CHECK_BCC
	bcc = pstLinkData->mPacketBuf[pstLinkData->mReceivedindex - 2];
	PACKET_BCC(pstLinkData->mPacketBuf);
	if(bcc != pstLinkData->mPacketBuf[OFFSET_BCC(pstLinkData->mPacketBuf)])
	{
#ifdef SER_DEBUG
		PutrsUART("VALIDATE_CMD - CheckSum error\r\n");
#endif
		return CHECKSUM_ERROR;
	}
#endif
	else
	{
#ifdef SER_DEBUG
		PutrsUART("VALIDATE_CMD - All data received OK!\r\n");
#endif
		return DATA_RECEIVED_OK;
	}
}

/*
*------------------------------------------------------------------------------
* void extractPacketData(PC_LINK_COMMAND *pstLinkData)
*
* Summary	: Get the command and data bytes for the packet
*
* Input		: PC_LINK_COMMAND *pstLinkDta - pointer to the received pc link data
*
* Output	: None 
*------------------------------------------------------------------------------
*/
void extractPacketData(PC_LINK_COMMAND *pstLinkData)
{
	UINT8 index;
	// Get the device id
	stLinkComm.mDevId = pstLinkData->mPacketBuf[STATION];
	// Get the command
	stLinkComm.mCommand = pstLinkData->mPacketBuf[COMMAND];
	// Get data packet size
	stLinkComm.mSize = (pstLinkData->mPacketBuf[DATALENGTH] - 1);
	// Get the data in the packet
	/*
	for(index = 0;index < stLinkComm.mSize;index++)
	{
		stLinkComm.mData[index] =  pstLinkData->mPacketBuf[DATA + index];
	}
	*/
	memcpy((UINT8*)&stLinkComm.mData,(const UINT8*)pstLinkData->mPacketBuf + DATA ,stLinkComm.mSize); 
}


/*
*------------------------------------------------------------------------------
* void SetDeviceID(PC_LINK_COMMAND_STRUCT *pstExtData)
*
* Summary	: Set the device number with the supplied num 
*
* Input		: PC_LINK_COMMAND_STRUCT *pstExtData - pointer to extracted data from the link
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void SetDeviceID(PC_LINK_COMMAND_STRUCT *pstExtData)
{
	MyDevID = pstExtData->mData[0];
	SendCmdResponce(RES_COMM_OK,NULL,0);
}	

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/
/*
*------------------------------------------------------------------------------
* void SendCmdResponce(UINT8 status, UINT8 *data, UINT8 len)
*
* Summary	: Sends response to the link command form PC
*
* Input		: UINT8 status - status info for the received command
* 			  UINT8 *data -  response data
* 			  UINT8 len -  response data length
*
* Output	: None
* Note		: Command Response Format: (Device to PC)
*
*
*------------------------------------------------------------------------------
*/
void SendCmdResponce(UINT8 status, UINT8 *data, UINT8 len)
{
	UINT8  i = 0;
	UINT8 packetLength = 0;
	UINT8 checksum = 0;
	UINT8 packetHeader[4]={RESP_SOF , DEVICE_ID };
	
	packetHeader[2] = len + 1;
	packetHeader[3] = status;

	for(i = 0 ; i < 4 ; i++)
	{
		UART_write( packetHeader[i]);
	
		
	}
	checksum = packetHeader[1] ^ packetHeader[2] ^ packetHeader[3];	

	for( i = 0 ; i < len ; i++ )
	{
		UART_write(data[i]);
		checksum ^= data[i];
	}
	
	UART_write( checksum );
	UART_write(RESP_EOF);
	UART_transmit();
	
	

/*
	UINT8 packetBuf[RECEIVER_MAX_PAKET_SIZE] = {SOF, MyDevID,};
	memset(packetBuf + 2, sizeof(packetBuf - 2), 0);
	packetBuf[DATALENGTH] = len + 1;
	packetBuf[COMMAND] = status;
	if(data) 
	{
		memcpy((UINT8*)packetBuf + DATA,(const UINT8*)data, len);
	}
	PACKET_BCC(packetBuf);
	packetBuf[OFFSET_ETX(packetBuf)] = EOF;
/*
	for(index = 0 ; index < 3; index++) 
	{
		PutcUARTRam(packetBuf[index+1]);
	}


	for(index = 0 ; index < PACKET_LEN(packetBuf); index++) 
	{
		PutcUARTRam(packetBuf[index]);			//Send data
	}
*/

/*

	packetLength = PACKET_LEN(packetBuf);
	for(index = 0 ; index < packetLength; index++) 
	{
		UART_write(packetBuf[index]);
	}
*/
}

/*
*------------------------------------------------------------------------------
* UINT8 CheckPCLinkDataStatus(void)
*
* Summary	: Checke whether the Link data is available or not
*
* Input		: None
*
* Output	: UINT8 return command receive ISR status
*		            
*------------------------------------------------------------------------------
*/
UINT8 CheckPCLinkDataStatus(void)
{
	return stLinkData.mReceiveResponse;
}

/*
*------------------------------------------------------------------------------
* void ResetPCLink(void)
*
* Summary	: Set the PC Link state to idle and reset the flags and
*			  char conunters
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/
void ResetPCLink(void)
{
	stLinkData.mReceiveResponse = IDLE;
	stLinkData.mReceivedindex = 0;
	stLinkData.mLeftBytes = 3;
}



/*
*------------------------------------------------------------------------------
* void InitializePCLink(void)
*
* Summary	: Initializes the Uart as 19200 baud,8N1 .
*
*
* Input		: None
*
* Output	: None
*
* Note		:
*
* Desired Baud Rate = FOSC/(64 ([SPBRGH:SPBRG] + 1)) - if USART_BRGH_LOW
* Desired Baud Rate = FOSC/(16 ([SPBRGH:SPBRG] + 1)) - if USART_BRGH_HIGH
* Solving for SPBRGH:SPBRG:
* X = ((FOSC/Desired Baud Rate)/16) - 1
*   = ((48000000/19200)/16) - 1
*   = 155.25
*	= 155
* Calculated Baud Rate = 48000000/(16 (155 + 1))
*                      = 19230.769
*					   = 19200
* X = ((FOSC/Desired Baud Rate)/64) - 1
*   = ((48000000/9600)/64) - 1
*   = 77.125
*	= 77
*------------------------------------------------------------------------------
*/
void InitializePCLink(void)
{
	int i ; 
	MyDevID = DEVICE_ID;
	ResetPCLink();


	// Setup task to run


}





/*
*------------------------------------------------------------------------------
* rom void PCLinkTask(void)
*
* Summary	: Handel all Uart Link related functions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
rom void PCLinkTask(void)
{
	UINT8 data , res;
	UINT8 log_length;

/*
	ENTER_CRITICAL_SECTION()
	res = CheckPCLinkDataStatus();
	EXIT_CRITICAL_SECTION()
*/
	if( UART_hasData() == FALSE)
	{
		return;	
	}
	data = UART_read();
	//putCharToLcd('C');

#ifdef __LOOP_BACK__
//#include "lcd_driver.h"
	{
    	PutcUARTRam(data);			//Send data
	}
#else
	
	stLinkData.mLeftBytes --;
	if(stLinkData.mReceivedindex == STX && data == SOF)
	{
		stLinkData.mPacketBuf[STX] = data;
		stLinkData.mReceivedindex ++;
		stLinkData.mReceiveResponse = STARTED;
	}
	else
	if (stLinkData.mReceivedindex == STX && data != SOF)
	{
		stLinkData.mReceiveResponse = ERROR;
		ResetPCLink();	//incorrect packet header reset receiver
		
	}
	else
	if(stLinkData.mReceivedindex == STATION && data == MyDevID)
	{
		stLinkData.mPacketBuf[STATION] = data;
		stLinkData.mReceivedindex ++;
	}
	else
	if (stLinkData.mReceivedindex == STATION && data != MyDevID)
	{
		stLinkData.mReceiveResponse = ERROR;
		ResetPCLink();	//data is not meant for this station 
	}
	else
	if(stLinkData.mReceivedindex == DATALENGTH && data <= MAX_DATALENGTH_VAL + 1)
	{
		stLinkData.mPacketBuf[DATALENGTH] = data;
		stLinkData.mReceivedindex ++;
		stLinkData.mLeftBytes = data + 2;
	}
	else
	if(stLinkData.mReceivedindex == DATALENGTH && ((data > MAX_DATALENGTH_VAL + 1) || (data == 0)))
	{
		stLinkData.mReceiveResponse = ERROR;
	}
	else
	if((stLinkData.mLeftBytes >0) && (stLinkData.mReceivedindex > DATALENGTH))
	{
		stLinkData.mPacketBuf[stLinkData.mReceivedindex ++] = data;

	}
	else
	if((stLinkData.mLeftBytes == 0) &&(stLinkData.mReceivedindex > DATALENGTH))
	{
		stLinkData.mPacketBuf[stLinkData.mReceivedindex ++] = data;
		stLinkData.mReceiveResponse = COMPLETED;
	}

	if(COMPLETED == stLinkData.mReceiveResponse)
	{

		//putCharToLcd('C');
		// Check the received data for errors
		if(DATA_RECEIVED_OK != validateReceivedCommand(&stLinkData))
		{
			// Reset the receive IST state and data counters, and send communicaton error response
			//ResetReceiver();

			ResetPCLink();
			SendCmdResponce(RES_COMM_ERR,NULL,0);
		}
		else
		{
			// Get the required information  
			extractPacketData(&stLinkData);
			//displayExtractPacketData(&stLinkComm);
			//ResetReceiver();
			ResetPCLink();
			switch(stLinkComm.mCommand)
			{
				case CMD_GET_DEV_ID:
				{
					// Send my device number
					SendCmdResponce(RES_COMM_OK,&MyDevID,1);
				}
				break;

				case CMD_GET_COMM_STATUS:
				{
					UINT8 temp = stLinkComm.mData[0];
					stLinkComm.mData[0] = stLinkComm.mData[1];
					stLinkComm.mData[1] = temp;
					SendCmdResponce(RES_COMM_OK,(UINT8*)&(stLinkComm.mData),2);
				}
				break;
				

				case CMD_GET_STATUS_LOG:
				{
					UINT8 *data;
					//putCharToLcd('L');
					log_length = getStatusLog(&data);
					SendCmdResponce(RES_COMM_OK , data , log_length );


				}
				break;


				case CMD_RAISE_ISSUE:
				{
					UINT8 temp = stLinkComm.mData[0];
					UINT8 status = RES_COMM_OK;
					if( IAS_updateIssueInfo(temp, RAISED) == FALSE)
					{
						status = RES_COMM_ERR;
					}
					SendCmdResponce(status , NULL , 0 );


				}
				break;

				case CMD_RESOLVE_ISSUE:
				{
					UINT8 temp = stLinkComm.mData[0];
					UINT8 status = RES_COMM_OK;
					if( IAS_updateIssueInfo(temp, RESOLVED) == FALSE)
					{
						status = RES_COMM_ERR;
					}
					SendCmdResponce(status , NULL , 0 );

				}
				break;

				
				
				default:
				{
					// Send error response
					SendCmdResponce(RES_BAD_COMMAND,NULL,0);
#ifdef SER_DEBUG
					PutrsUART("PC_LINK_TASK - Bad command !\r\n");
#endif
				}
				break;
			}
		}
	}
	else
	if(ERROR == res)
	{
		// Reset the receive IST state and data counters, and send communicaton error response
		ResetPCLink();
	}

#endif
}

/*
*  End of pc_com_interface.c
*/
