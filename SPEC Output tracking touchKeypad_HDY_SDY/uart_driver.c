
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <delays.h>
#include <usart.h>
#include <string.h>
#include "typedefs.h"
#include "device.h"
#include "uart_driver.h"



/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/
/*

enum RECEIVER_STATE
{
	UART_RX_STATE_IDLE,
	UART_RX_STATE_RECV_START,
	UART_RX_STATE_WAIT
};
*/
/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

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


/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/
#pragma idata uartdata
UART uart = {0,0,0,{0}};

#pragma idata
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

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void  UartReceiveHandler(void)
*
* Summary	: Handle the recepion of data throug UART
*
* Input		: None
*
* Output	: None
*
* Note		: PC Command Fromat: (PC to Device)
*
* Field:	SOF     STATIONID  DATALEN   CMD/STATUS    DATAN           BCC     EOF
*
* offset:	0	    1	       2	     3		       4               N+4     N+5
*
* value:	0xAA	0x00	   N+1	     CMD	       DATA[0..N-1]    BCC     0xBB
*
* Desc:		BCC = STATIONID ^ DATALEN ^ CMD/STATUS ^ DATA[0] ^ DATA[1]...^... DATA[N-1]
*
*------------------------------------------------------------------------------
*/

#pragma interrupt UartReceiveHandler
void  UartReceiveHandler(void)
{
  	volatile UINT8 data;




  	// Get the character received from the UART
  	data = ReadUSART();

    uart.rxBuff[uart.rxBuffIndex++] = data;

	if( uart.rxBuffIndex >= RX_PACKET_SIZE)
	{
		uart.rxBuffIndex = 0;
	}
	uart.rxDataCount++;

	// Clear the interrupt flag
    PIR1bits.RCIF = 0;



}


/*
*
*
*/

#pragma interrupt UartTransmitHandler
void UartTransmitHandler(void)
{
	volatile UINT8 data ;
	if( uart.txDataCount == 0 )	//check whether there is data to be transmitted
	{
		while(BusyUSART());
		TX_EN = 0;				//disable RS485 control
		TXSTAbits.TXEN = 0;		//disable transmission
		return;
	}
	data = uart.txBuff[uart.txDataIndex];
	TXREG = data;	//transmit
	uart.txDataIndex++;		//increase the dataIndex to point to the next data to be transmitted
	if( uart.txDataIndex >= TX_PACKET_SIZE)		//on rollover
	{
		uart.txDataIndex = 0;		//reset
	}
	if( uart.txDataCount > 0)
	uart.txDataCount--;		//decrement the data count to indicate transmission

}
	

	

/*
*------------------------------------------------------------------------------
* void UART_init(void)
*
* Summary	: Initialize Uart for communication with PC
*			  char conunters
*				Transimission and Reception interrupts are enabled
*				
* 				Refer PIC 18F4520 datasheet Table 18-3.
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/
void UART_init(void)
{
	UINT8 uartConfig= USART_TX_INT_OFF &
				USART_RX_INT_ON &
				USART_ASYNCH_MODE &
				USART_EIGHT_BIT &
				USART_CONT_RX &
				USART_BRGH_HIGH;
// configure USART
	OpenUSART( uartConfig ,
				//77);
				//129 );
				//10 );
				//19 );
				//64 );
				52);	

	
	TXSTAbits.TXEN = 1;	//enable transmission
	TXSTAbits.CSRC = 0;

  	// Make receive interrupt high priority
  	IPR1bits.RCIP = 1;

}


/*
*------------------------------------------------------------------------------
* BOOL UART_write(UINT8 data)
*
* Summary	: Fill the transmit buffer of the uart with data
* Input		: data to be transmitted which is stored in the txBuff
*
* Output	: Boolean indicating success or failure
* 
*------------------------------------------------------------------------------
*/
BOOL UART_write(UINT8 data)
{
	while(BusyUSART());					//wait for current transmission if any
	//DISABLE_UART_TX_INTERRUPT();		//disable the transmit interrupt
	uart.txBuff[uart.txBuffIndex++] = data;	//store the data in the tx buffer
	uart.txDataCount++;		//increment the data count
	if(uart.txBuffIndex >= TX_PACKET_SIZE)	//check for overflow
	{
		uart.txBuffIndex = 0;
	}

	//ENABLE_UART_TX_INTERRUPT();
		
	return TRUE;
}




/*
*------------------------------------------------------------------------------
* BOOL UART_transmit(void)
*
* Summary	: Fill the transmit buffer of the uart with data
* Input		: data to be transmitted which is stored in the txBuff
*
* Output	: Boolean indicating success or failure
* 
*------------------------------------------------------------------------------
*/
BOOL UART_transmit(void)
{
/*
	DISABLE_UART_TX_INTERRUPT();		//disable the transmit interrupt
	if( TXSTAbits.TXEN == 0 )		//data count ==1 requires initiation of transmission
	{
		while(BusyUSART());			//wait for current transmission to complete 
		TX_EN = 1;					//enable RS485 transmission
		TXSTAbits.TXEN = 1;		//enable uart transmission
		ENABLE_UART_TX_INTERRUPT();

		return TRUE;
	}
		
	ENABLE_UART_TX_INTERRUPT();
	return FALSE;
*/

	UINT8 i,data;
	TX_EN = 1;	//enable the tx control for rs485 communication
	DelayMs(1);	//required based on hardware testing
	while(uart.txDataCount > 0 )
	{
		
		
		data = uart.txBuff[uart.txDataIndex];

		while (!TXSTAbits.TRMT);	//check whether ready to transmit
		TXREG=data;					//load new data for transmission
	
		while (!TXSTAbits.TRMT);	//check whether ready to transmit
		uart.txDataIndex++;		//increase the dataIndex to point to the next data to be transmitted
		if( uart.txDataIndex >= TX_PACKET_SIZE)		//on rollover
		{
			uart.txDataIndex = 0;		//reset
		}
		if( uart.txDataCount > 0)
		uart.txDataCount--;		//decrement the data count to indicate transmission
		
	}
	TX_EN = 0;	//disable the tx control for rs485 communication
	DelayMs(1);	//required based on hardware testing

}
	




/*
*------------------------------------------------------------------------------
BOOL UART_hasData(void)
*
* Summary	: indicates whether data has been received from the uart
* Input		: None
*
* Output	: the data from the rxBuff at index = rxDataIndex
* 
*------------------------------------------------------------------------------
*/
BOOL UART_hasData(void)
{
	BOOL result = FALSE;
	DISABLE_UART_RX_INTERRUPT();
	if(uart.rxDataCount > 0)
		result = TRUE; 
	ENABLE_UART_RX_INTERRUPT();
	
		
	return result;
}





/*
*------------------------------------------------------------------------------
UINT8 UART_read(void)
*
* Summary	: return the data from the rxBuff 
* Input		: None
*
* Output	: the data from the rxBuff at index = rxDataIndex
* 
*------------------------------------------------------------------------------
*/
UINT8 UART_read(void)
{
	UINT8 data;
	DISABLE_UART_RX_INTERRUPT();
	data = uart.rxBuff[uart.rxDataIndex++];
	if( uart.rxDataIndex >= RX_PACKET_SIZE)
	{
		uart.rxDataIndex = 0;
	}
	if( uart.rxDataCount > 0 )
		--uart.rxDataCount;
	ENABLE_UART_RX_INTERRUPT();

		
	return data;
}






/*
*------------------------------------------------------------------------------
* void PutrsUART(const rom char *data)
*
* Summary	: Send the rom character string through UART
*
* Input		: const rom char *data - pointer to the null terminated string rom.
*
* Output	: None
*------------------------------------------------------------------------------
*/
void PutrsUART(const rom char *data)
{
	TX_EN = 1;	//enable the tx control for rs485 communication
	Delay10us(1);
  do
  {
	// Wait till the last bit trasmision
    while(BusyUSART());
	// Transmit a byte
    putcUSART(*data++);
  } while( *data != '\0');

	TX_EN = 0;	//disable the tx control for rs485 communication
	Delay10us(1);
}

/*
*------------------------------------------------------------------------------
* void PutrsUARTRam(const UINT8 *data)
*
* Summary	: Send the ram character string through UART
*
* Input		: const UINT8 *data - pointer to the null terminated string in ram.
*
* Output	: None
*------------------------------------------------------------------------------
*/
void PutrsUARTRam(const UINT8 *data)
{
	TX_EN = 1;	//enable the tx control for rs485 communication
	Delay10us(1);
  do
  {
	// Wait till the last bit trasmision
    while(BusyUSART());
	// Transmit a byte
    putcUSART(*data++);
  } while( *data != '\0');

	TX_EN = 0;	//disable the tx control for rs485 communication
	Delay10us(1);

}

/*
*------------------------------------------------------------------------------
* void PutcUARTRam(const UINT8 data)
*
* Summary	: Send the ram character through UART
*
* Input		: const char *data - pointer to the null terminated string in ram.
*
* Output	: None
*------------------------------------------------------------------------------
*/
void PutcUARTRam(const UINT8 data)
{
/*
	// Wait till the last bit trasmision
    while(BusyUSART());
	TX_EN = 1;	//enable the tx control for rs485 communication
	DelayMs(10);

	// Transmit a byte
    putcUSART(data);

	while(BusyUSART());
	TX_EN = 0;	//disable the tx control for rs485 communication
	DelayMs(10);

*/

	
	

}

/*
*  End of uart_driver.c
*/
