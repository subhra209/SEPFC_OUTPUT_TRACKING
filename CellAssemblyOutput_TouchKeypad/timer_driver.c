/*
*------------------------------------------------------------------------------
* timer_driver.c
*
* timer_driver module.
*
* (C)2008 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: timer_driver.c
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 28/11/2008
*------------------------------------------------------------------------------
*
* Revision 1.0 28/11/2008 Sam
* Demo release
* Revision 0.0 03/10/2008 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


#include <timers.h>
#include "device.h"
#include "typedefs.h"
#include "uart_driver.h"
#include "timer_driver.h"
//#include "keypad_driver.h"
#include "lcd_driver.h"
#include "keypad.h"

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

enum 
{
	TIMER_TIMEOUT 	= 10,
	APP_TIMEOUT 	= 1000,
	HB_TIMEOUT 		= 500,
	KEYPAD_TIMEOUT  = 20,
	TIMEOUT_COUNT 	= (APP_TIMEOUT/TIMER_TIMEOUT)
	
	
};
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

UINT32 AppTimestamp = 0;
/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

 UINT16 hbUpdate_count = 0;
static UINT32 TimerUpdate_count = 0;
 UINT8 keypadUpdate_count = 0;
 UINT16 appUpdate_count = 0;
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

void timer0ISR(void);
void timer3ISR(void);

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/
UINT32 GetAppTime(void);
void ResetAppTime(void);
/*
*------------------------------------------------------------------------------
* void timer0ISR(void)

* Summary	: The timer0 interrupt trigeres for every 1ms.and this is the time
*			  base for schedular
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/

/*
 * For PIC18xxxx devices, the low interrupt vector is found at 000000018h.
 * Change the default code section to the absolute code section named
 * low_vector located at address 0x18.
 */
/*
#pragma code low_vector=0x18
void low_interrupt (void)
{

   	// Inline assembly that will jump to the ISR.
    	_asm GOTO timer0ISR _endasm
}
*/

#pragma code high_vector = 0x08
void high_interrupt (void)
{
	/*
   	* Inline assembly that will jump to the ISR.
   	*/
/*
	if( PIR1bits.TXIF == 1)
	{
		_asm GOTO UartTransmitHandler _endasm
	}
*/
	if(INTCONbits.TMR0IF == 1) 
	{
  		_asm GOTO timer0ISR _endasm
	}
/*
	if( PIR2bits.TMR3IF == 1 )
	{
		_asm GOTO 	timer3ISR _endasm
	}
*/
	if(PIR1bits.RCIF == 1)
	{
			//putCharToLcd('I');
		_asm GOTO UartReceiveHandler _endasm
	}




}
/*
 * Returns the compiler to the default code section.
 */
#pragma code

/*
 * Specifies the function timer0ISR as a low-priority interrupt service
 * routine. This is required in order for the compiler to generate a
 * RETFIE instruction instead of a RETURN instruction for the timer0ISR
 * function.
 */
//#pragma interruptlow timer0ISR
#pragma interrupt timer0ISR
/*
 * Define the timer_isr function. Notice that it does not take any
 * parameters, and does not return anything (as required by ISRs).
 */
void timer0ISR(void)
{
  	/*
   	* Clears the TMR0 interrupt flag to stop the program from processing the
   	* same interrupt multiple times.
   	*/
  	INTCONbits.TMR0IF = 0;
	TimerUpdate_count++;
	if(TimerUpdate_count == TIMEOUT_COUNT)
	{
		AppTimestamp++;
		TimerUpdate_count = 0;
	}
	hbUpdate_count++;
	appUpdate_count++;
	keypadUpdate_count++;

	// Reload value for 5ms overflow
	WriteTimer0(TICK_PERIOD);
}




#pragma code
/*
*------------------------------------------------------------------------------
* void InitTimer0(void)

* Summary	: Initialize timer0 for schedular base
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void InitTimer0(void)
{
	// Enable the TMR0 interrupt,16bit counter
	// with internal clock,No prescalar.
   	OpenTimer0 (TIMER_INT_ON & T0_SOURCE_INT & T0_16BIT & T0_PS_1_1);
	// Reload value for 5ms overflow
	WriteTimer0(TICK_PERIOD);


	INTCON2bits.TMR0IP = 1;
}

UINT32 GetAppTime(void)
{	
	UINT32 temp;

	DISABLE_TMR0_INTERRUPT();
	temp  = AppTimestamp;
	ENABLE_TMR0_INTERRUPT();
	return temp;
}

void ResetAppTime(void)
{
	ENTER_CRITICAL_SECTION();
	AppTimestamp = 0;
	EXIT_CRITICAL_SECTION();
}

/*
*  End of timer_driver.c
*/