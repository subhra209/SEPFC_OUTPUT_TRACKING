


/*
*------------------------------------------------------------------------------
* main.c
*
* main application specific module.
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
* Include Files
*------------------------------------------------------------------------------
*/

#include "device.h"
#include <timers.h>			// Timer library functions
#include <delays.h>			// Dely library functions
#include <string.h>			// String library functions
#include "timer_driver.h"	// Timer related functions
#include "uart_driver.h"
#include "lcd_driver.h"
//#include "keypad_driver.h"
#include "ui.h"
#include "communication.h"
#include "ias.h"
#include "keypad.h"
/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/

#pragma config OSC     = INTIO67
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOREN    = ON
#pragma config BORV     = 3
#pragma config WDT      = OFF
#pragma config WDTPS    = 512	//32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//#pragma config ICPRT  = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = ON
#pragma config CP2      = ON
#pragma config CP3      = ON
#pragma config CPB      = ON
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
//#pragma config WRT3   = OFF
#pragma config WRTB     = OFF//N       // Boot Block Write Protection
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF

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
* Public Functions
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
char uartData = 'A';
 extern UINT8 keypadUpdate_count;
extern UINT16 hbUpdate_count;
extern UINT8 appUpdate_count;
static BOOL hbState = 0;
#define HB_COUNT 	100	
#define KEYPAD_COUNT  10

void main(void)
{
	UINT8 i;
	InitializeBoard();
	InitTimer0();

	//InitializeKeypad();
	InitializeLcd();
	KEYPAD_init();


#ifdef LCD_TEST	

	for(i=0;i<32;i++)
	{
		putCharToLcd(i +'A');
		DelayMs(200);
		ClrWdt();
	}

	clearLCD();
	DelayMs(500);

#endif
	

	ClrWdt();


	UI_init();
	APP_init();
	ENABLE_GLOBAL_INT();

#ifdef UART_TEST

	for(i= 0; i<26 ; i++)
	{
		UART_write(i + 'A');
		 UART_transmit();
	}

#endif

   	while(TRUE)
    {
		UI_task();
		ClrWdt();

		
		COM_task();
		ClrWdt();

		if( appUpdate_count > 100 )
		{
			APP_task();
			appUpdate_count = 0;
		}
		ClrWdt();

		if(keypadUpdate_count > KEYPAD_COUNT)
		{
			KEYPAD_task();
			keypadUpdate_count = 0;
		}

		if(hbUpdate_count >= HB_COUNT)
		{
			hbState ^= SWITCH_ON;
			HEART_BEAT = hbState;
			hbUpdate_count = 0;
		}
				
    }
}

/*
*  End of main.c
*/