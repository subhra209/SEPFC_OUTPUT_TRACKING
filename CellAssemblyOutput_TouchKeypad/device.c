
/*
*------------------------------------------------------------------------------
* device.c
*
* Board specipic drivers module(BSP)
*
* (C)2009 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.c
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 14/07/2009
*------------------------------------------------------------------------------
*
* Revision 0.0 14/07/2009 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "device.h"
#include "typedefs.h"

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
* void InitializeBoard(void)

* Summary	: This function configures all i/o pin directions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void InitializeBoard(void)
{
	// set all anolog channels as Digital I/O
	ADCON1 = 0x0D;
	OSCCON |= 0x60;
	OSCTUNEbits.PLLEN = 1;		//pll  enable

/*	
	// Configure keypad input
	KBD_ROW0_DIR = PORT_IN;
	KBD_ROW1_DIR = PORT_IN;
	KBD_ROW2_DIR = PORT_IN;
	KBD_ROW3_DIR = PORT_IN;
	KBD_COL0_DIR = PORT_OUT;
	KBD_COL1_DIR = PORT_OUT;
	KBD_COL2_DIR = PORT_OUT;
	KBD_COL3_DIR = PORT_OUT;
*/

	//KEYPAD PORT
	KEYPAD_PORT = PORT_IN;


	// Rs485 Direction Control
	
	TX_EN_DIR = PORT_OUT;
	TX_EN = SWITCH_OFF;
	SER_TX_DIR = PORT_OUT;
	SER_RX_DIR = PORT_IN;
	
	// Configure Buzzer output
	BUZ_OP_DIR 			= PORT_OUT;	
	BUZ_OP 				= SWITCH_OFF;

	LAMP_GREEN_DIR		= PORT_OUT;
	LAMP_GREEN 			= SWITCH_ON;
	
	LAMP_RED_DIR		= PORT_OUT;
	LAMP_RED 			= SWITCH_OFF;

	LAMP_YELLOW_DIR		= PORT_OUT;
	LAMP_YELLOW 		= SWITCH_OFF;
	
	// Configure heart beat LED output 
	HEART_BEAT_DIR = PORT_OUT;
	HEART_BEAT = ~SWITCH_OFF;	
	
	// Configure LCD port as outputs
	LCD_D7_DIR = PORT_OUT;
	LCD_D6_DIR = PORT_OUT;
	LCD_D5_DIR = PORT_OUT;
	LCD_D4_DIR = PORT_OUT;
	LCD_E_DIR  = PORT_OUT;
	LCD_RW_DIR = PORT_OUT;
	LCD_RS_DIR = PORT_OUT;

}







/*
*  End of device.c
*/
