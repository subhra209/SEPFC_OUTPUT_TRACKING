#include "board.h"
#include "timer.h"
#include "uart_driver.h"
#include "Externalinterrupt.h"

/*
 * For PIC18xxxx devices, the low interrupt vector is found at 000000018h.
 * Change the default code section to the absolute code section named
 * low_vector located at address 0x18.
 */


#pragma code high_vector = 0x08
void high_interrupt (void)
{
	/*
   	* Inline assembly that will jump to the ISR.
   	*/
#ifdef __18F8722_H
	if(PIR1bits.RC1IF == 1)
	{
		_asm GOTO Uart1_ReceiveHandler _endasm
	}



#else

	if(PIR1bits.RCIF == 1)
	{
		_asm GOTO UartReceiveHandler _endasm
	}
#endif

	if(INTCONbits.TMR0IF == 1)
	{
  		_asm GOTO TMR0_ISR _endasm
	}

	if(PIR1bits.TMR1IF == 1)
	{
		_asm GOTO TMR1_ISR _endasm
	}

     if(INTCONbits.INT0IF == 1)
	{
		_asm GOTO RB0INT_ISR _endasm
	}
	if(INTCON3bits.INT1IF == 1)
	{
		 _asm GOTO RB1INT_ISR _endasm
	}


}
/*
#pragma code low_vector=0x18
void low_interrupt (void)
{
	if(PIR1bits.TMR1IF == 1)
	{
		_asm GOTO TMR1_ISR _endasm
	}
}
*/
/*
*------------------------------------------------------------------------------
* void EnableInterrupts(void)

* Summary	: Enable interrupts and related priorities
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void EnableInterrupts(void)
{
	// Enable interrupt priority
  	RCONbits.IPEN = 1;
 	// Enable all high priority interrupts
  	INTCONbits.GIEH = 1;
	INTCONbits.PEIE = 1;	//enable low priority interrupts
}