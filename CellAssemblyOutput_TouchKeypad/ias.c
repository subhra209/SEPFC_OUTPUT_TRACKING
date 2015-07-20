
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/
#include "config.h"
#include "device.h"
#include "timer_driver.h"
#include "communication.h"
#include "ias.h"
//#include "Keypad_driver.h"
#include "lcd_driver.h"
#include "string.h"
#include "eep.h"
#include "ui.h"
#include "keypad.h"

enum
{
	MAX_PACKETS = 6,
	TIMEOUT = 1000
};

//#define SIMULATION
/*
*------------------------------------------------------------------------------
* Structures
*------------------------------------------------------------------------------
*/
typedef struct 
{
	UINT8 destination;
	UINT8 length;
	UINT8 cmd_resp;
	UINT8 data[MAX_KEYPAD_ENTRIES + 1];

	BOOL sent;
	
}PACKET;
																			//Object to store packet
																			//Object to store log entries

typedef struct 
{
	PACKET packets[MAX_PACKETS];
	UINT8 packetIndex;

	INT32 timeout;
	UINT8 currentLine;
	BOOL inTimeout;

}APP;																			//This object contains all the varibles used in this application




/*
*------------------------------------------------------------------------------
* Variables
*------------------------------------------------------------------------------
*/
#pragma idata iasdata
APP app = {0};
#pragma idata



/*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/


UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket);


/*
*------------------------------------------------------------------------------
* void IAS-init(void)
*------------------------------------------------------------------------------
*/

void APP_init(void)
{

	UINT8 i;
	for( i = 0 ; i< MAX_PACKETS; i++ )
	{
			app.packets[i].sent = TRUE;
			app.packets[i].length = 0;
	}
	app.timeout = -1;
	COM_init(CMD_SOP , CMD_EOP ,RESP_SOP , RESP_EOP , APP_comCallBack);
}


/*
*------------------------------------------------------------------------------
* void IAS-task(void)
*------------------------------------------------------------------------------
*/
void APP_task(void)
{
	UINT8 i;
	for( i = 0 ; i< MAX_PACKETS; i++ )
	{
		if( app.packets[i].sent == FALSE)
		{
			COM_txPacket((UINT8*)&app.packets[i], app.packets[i].length,COM_CMD );
			app.packets[i].sent = TRUE;
			app.packets[i].length = 0;
		}
	}
	if( app.inTimeout == TRUE )
	{
		app.timeout-- ;
		if( app.timeout == 0 )
		{
			UI_reset();
			app.inTimeout = FALSE;
		}
		
	}
	
}


void APP_startTransition(UINT8*data)
{
	UINT8 i,j;

	for( i = 0 ; i< MAX_PACKETS; i++ )
	{
		if( app.packets[i].sent == TRUE)
		{
			break;
		}
	}
	app.packets[i].length++;
	app.packets[i].destination = 1;
	app.packets[i].length++;
	app.packets[i].cmd_resp = CMD_START_TRANSITION;
	app.packets[i].length++;
	app.currentLine = app.packets[i].data[0] = *data;
	app.packets[i].length++;
	app.packets[i].sent = FALSE;

	app.timeout = TIMEOUT;
	app.inTimeout = TRUE;
}

void APP_setReference(UINT8*data, UINT8 length)
{
	UINT8 i,j;

	for( i = 0 ; i< MAX_PACKETS; i++ )
	{
		if( app.packets[i].sent == TRUE)
		{
			break;
		}
	}
	app.packets[i].length++;
	app.packets[i].destination = 1;
	app.packets[i].length++;
	app.packets[i].cmd_resp = CMD_SET_REFERENCE_CODE;
	app.packets[i].length++;
	app.packets[i].data[0] = app.currentLine;
	for( j = 1; j <= length ;j++,data++)
	{
		app.packets[i].data[j] = *data;
	}
	app.packets[i].length+=j;
	app.packets[i].sent = FALSE;
	app.inTimeout = FALSE;

}


void APP_setBreak()
{
	UINT8 i,j;

	for( i = 0 ; i< MAX_PACKETS; i++ )
	{
		if( app.packets[i].sent == TRUE)
		{
			break;
		}
	}
	app.packets[i].length++;
	app.packets[i].destination = 1;
	app.packets[i].length++;
	app.packets[i].cmd_resp = CMD_SET_BREAK;
	app.packets[i].length++;
	app.packets[i].data[0] = app.currentLine;
	app.packets[i].length++;
	app.packets[i].sent = FALSE;
	app.inTimeout = FALSE;

}


void APP_setOperators(UINT8*data, UINT8 length)
{
	UINT8 i,j;

	for( i = 0 ; i< MAX_PACKETS; i++ )
	{
		if( app.packets[i].sent == TRUE)
		{
			break;
		}
	}
	app.packets[i].length++; 
	app.packets[i].destination = 1;
	app.packets[i].length++;
	app.packets[i].cmd_resp = CMD_SET_OPERATORS;
	app.packets[i].length++;
	app.packets[i].data[0] = app.currentLine;
	for( j = 1; j <=length ;j++,data++)
	{
		app.packets[i].data[j] = *data;
	}
	app.packets[i].length+=j;
	app.packets[i].sent = FALSE;
	app.inTimeout = FALSE;
}






UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket)
{


}
	
		

		
	

