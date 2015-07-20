
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
* File				: main.c
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 07/07/2009
*------------------------------------------------------------------------------
*
* Revision 0.0 07/07/2009 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "board.h"
#include "timer.h"	// Timer related functions
#include "uart_driver.h"
#include "communication.h"
#include "heartBeat.h"
#include "app.h"
#include "mmd.h"
#include "rtc_driver.h"



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
#pragma config MODE 	= MC
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
//#pragma config PBADEN   = OFF
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


extern UINT32 TimerUpdate_count;
extern UINT16 keypadUpdate_count;


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
	void EnableInterrupts(void);
extern UINT16 heartBeatCount ;
extern UINT16 comUpdateCount ;
extern UINT16 mmdUpdateCount;
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

#define MMD_REFRESH_PERIOD	(65535 - 20000)
#define TICK_PERIOD	(65535 - 16000)


UINT8 writeTimeDateBuffer[] = {0x01, 0X57, 0x12, 0x03, 0x11, 0x02,0x14};
void main(void)
{
	UINT8 i,j;
	BOOL ledStrip_On = 0;
	BRD_init();
	HB_init();
    External_Init();
	MMD_init();  // Display initialization
    


	COM_init(CMD_SOP,CMD_EOP,RESP_SOP,RESP_EOP,APP_comCallBack);
	
	LinearKeyPad_init( );

	DelayMs(3000);

	InitializeRtc();	


	APP_init();




	TMR0_init(TICK_PERIOD,0);	//initialize timer0
	TMR1_init(MMD_REFRESH_PERIOD,MMD_refreshDisplay);


	EnableInterrupts();

#ifdef __SET_RTC__


	WriteRtcTimeAndDate(writeTimeDateBuffer);
#endif

	while(1)
	{

		if( comUpdateCount > 5 )
		{
		
			COM1_task();	
			//COM2_task();	
			comUpdateCount = 0;
		}
#ifdef TIME_DEBUG	
		if(  heartBeatCount >= 250 )
		{
#endif
			UpdateRealTimeClockTask();
			if( TimeClockUpdateRequired == TRUE)
			{
				HB_task();
			
				TimeClockUpdateRequired = FALSE;
			}

#ifdef TIME_DEBUG
		heartBeatCount = 0;
		}
#endif
		if(  heartBeatCount >= 250 )
		{
				APP_task();
				 heartBeatCount = 0;
		}

		if( mmdUpdateCount >= 5 )
		{
			MMD_task();
			mmdUpdateCount = 0;
		}
		
		if(keypadUpdate_count >= 10)
		{
			//LinearKeyPad_scan();
			keypadUpdate_count = 0;
		}


	}
}

/*
*  End of main.c
*/