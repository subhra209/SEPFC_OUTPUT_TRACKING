#include "device.h"
#include "config.h"
//#include "Keypad_driver.h"
#include "lcd_driver.h"
#include "string.h"
#include "ui.h"
#include "ias.h"
#include "keypad.h"

typedef enum
{
	UI_LINE = 0,
	UI_MENU,
	UI_REFERENCE,
	UI_OPERATORS,
	UI_SET_STAGE_ID,
	UI_PASSWORD,
	UI_ADMIN_ACTIVITY,
	UI_BREAK

}UI_STATE;

typedef struct _UI
{
	UI_STATE state;
	UINT8 buffer[MAX_INPUT_CHARS+1];
	UINT8 bufferIndex;
	UINT8 prevcode;
	UINT8 keyIndex;
	UINT8 input[MAX_INPUT_CHARS+1];
	UINT8 inputIndex;
}UI;





const rom UINT8 *UI_MSG[]=
		{"LINE:",
		"SET:",
		"REFRERENCE",
		"OPERATORS",
		"HDY",
		"SDY",
		"BREAK",
		"REF:",
		"OPERATORS:"};




static rom UINT8 keyMap[MAX_KEYS][MAX_CHAR_PER_KEY] = {
													{'1','-','1','-','1'},{'2','A','B','C','2'},{'3','D','E','F','3'},{'\x11','\x11','\x11','\x11','\x11'},
													{'4','G','H','I','4'},{'5','J','K','L','5'},{'6','M','N','O','6'},{'\x12','\x12','\x12','\x12','\x12'},
													{'7','P','Q','R','S'},{'8','T','U','V','8'},{'9','W','X','Y','Z'},{'\x08','\x08','\x08','\x08','\x08'},
													{'*','*','*','*','*'},{'0','0','0','0','0'},{'\x13','\x13','\x13','\x13','\x13'},{'\x0A','\x0A','\x0A','\x0A','\x0A'}
};												




#pragma idata ui_data
UI ui = {0,{0},0,0xFF,0,0};

#pragma idata



UINT8 mapKey(UINT8 scancode, UINT8 duration);
UINT8 getOperators(void);
void getData(void);
void clearUIBuffer(void);
void putUImsg(UINT8 msgIndex);
void setUImsg( UINT8 msgIndex );
void clearUIInput(void);



void UI_init(void)
{

	setbckspc('\x08');	//Indicates to LCD driver "\x08" is the symbol for backspace

	setUImsg(UI_MSG_MENU);

	clearUIBuffer();
	clearUIInput();
	ui.state = UI_MENU;
}

void UI_reset()
{
		setUImsg(UI_MSG_MENU);
		clearUIBuffer();
		clearUIInput();
		ui.state = UI_MENU;
}
	

void UI_task(void)
{

	UINT8 keypressed = 0xFF;
	UINT8 i;
	UINT8 duration, scancode;
	UINT8 uimsg;

	if(KEYPAD_read(&scancode, &duration) == FALSE)			//Check whether key has been pressed
	{
		return;
	}


	keypressed = mapKey(scancode,duration);				//Map the key

	if( keypressed == 0xFF)
	{
		return;
	}


	switch(ui.state)
	{
		case UI_LINE:															//UI_LINE
		{
			UINT8 line = 0;
			if( keypressed == '\x08')
			{
					setUImsg(UI_MSG_LINE);
					clearUIBuffer();
					clearUIInput();
					ui.state = UI_LINE;
			}			
			else if( keypressed == '\x0A')
			{
				if(ui.bufferIndex > 0)
				{
					APP_startTransition(ui.input);

					setUImsg(UI_MSG_MENU);
					clearUIBuffer();
					clearUIInput();
					ui.state = UI_MENU;
				}
			
			}

			else if( keypressed == '\x11')							//HDY	
			{
				if(ui.bufferIndex < 1 )
				{
					ui.input[ui.inputIndex] = 2;
					putUImsg(UI_MSG_HDY);
					ui.buffer[ui.bufferIndex++]= keypressed;
				}
			}

			else if( keypressed == '\x12')							//SDY
			{
				if(ui.bufferIndex < 1 )
				{
					ui.input[ui.inputIndex] = 3;
					putUImsg(UI_MSG_SDY);
					ui.buffer[ui.bufferIndex++] = keypressed;
				}
			}

		}
		break;

		case UI_MENU:

			if( keypressed == '\x08')
			{

					setUImsg(UI_MSG_MENU);
					clearUIBuffer();
					clearUIInput();
					ui.state = UI_MENU;
				
			}
			else if( keypressed == '\x0A')
			{
				if(ui.bufferIndex == 1)
				{
					ui.input[ui.inputIndex] = DEVICE_ADDRESS;
					switch( ui.buffer[ui.bufferIndex-1] )
					{
						case '1':
							APP_startTransition(ui.input);			
							setUImsg(UI_MSG_NAME);
							clearUIBuffer();
							clearUIInput();
							ui.state = UI_REFERENCE;
						return;
						case '2':
							APP_startTransition(ui.input);
							setUImsg(UI_MSG_OPS);
							clearUIBuffer();
							clearUIInput();
							ui.state = UI_OPERATORS;
						return;

					
						default:
						APP_startTransition(ui.input);
						setUImsg(UI_MSG_MENU);
						clearUIBuffer();
						clearUIInput();
						ui.state = UI_MENU;
						return;
					}			

				}
			
			}

			else if( keypressed == '\x11')
			{
				putUImsg(UI_MSG_REFERENCE);
				ui.buffer[ui.bufferIndex++] = '1';
			}
			else if( keypressed == '\x12')	
			{
				putUImsg(UI_MSG_OPERATORS);
				ui.buffer[ui.bufferIndex++] = '2';
			}
			else if( keypressed == '*')
			{
				putUImsg(UI_MSG_BREAK);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_BREAK;
			}
		break;

		case UI_REFERENCE:														//UI_REFERENCE
		if( keypressed == '\x08')
		{
			if(ui.bufferIndex > 0 )
			{
				putCharToLcd(keypressed);
				ui.bufferIndex--;
			}
			else
			{
				setUImsg(UI_MSG_MENU);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_MENU;
			}

		}

		else if( keypressed == '\x13')
		{
			if( ui.bufferIndex >= 0 )
			{
				getData();
				APP_setReference(ui.input,(ui.inputIndex) );
				setUImsg(UI_MSG_MENU);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_MENU;
			}
		}

		else
		{
			if(scancode == ui.prevcode)
			{
				if(duration < MIN_KEYPRESS_DURATION)
				{
					ui.bufferIndex--;
					ui.buffer[ui.bufferIndex] = keypressed;
					putCharToLcd('\x08');
					putCharToLcd(ui.buffer[ui.bufferIndex]);
					DelayMs(20);
					ui.bufferIndex++;
				}
				else
				{
					ui.buffer[ui.bufferIndex] = keypressed;
					putCharToLcd(ui.buffer[ui.bufferIndex]);
					DelayMs(20);
					ui.bufferIndex++;
				}
			}
			else
			{
				ui.buffer[ui.bufferIndex] = keypressed;
				putCharToLcd(ui.buffer[ui.bufferIndex]);
				DelayMs(20);
				ui.bufferIndex++;
			}
			ui.prevcode = scancode;
		}
		break;

		case UI_OPERATORS:													//OPERATORS
		if( keypressed == '\x08')
		{
			if(ui.bufferIndex > 0 )
			{
				putCharToLcd(keypressed);
				ui.bufferIndex--;
				ui.inputIndex--;
			}
			else
			{
			
				setUImsg(UI_MSG_MENU);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_MENU;
			}

		}
		else if( keypressed == '\x13')
		{
			if( ui.bufferIndex >= 0 )
			{
				
				getOperators();
				APP_setOperators(ui.input, ui.inputIndex);
				setUImsg(UI_MSG_MENU);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_MENU;
			}

		}
		else
		{
			if(ui.bufferIndex < 2)
			{	
				ui.buffer[ui.bufferIndex] = keypressed;
				putCharToLcd(ui.buffer[ui.bufferIndex]);
				//DelayMs(20);
				ui.bufferIndex++;			
			}
		}
		break;


		case UI_BREAK:													//OPERATORS
		if( keypressed == '\x08')
		{
				putCharToLcd(keypressed);
	
				setUImsg(UI_MSG_MENU);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_MENU;
		

		}
		else if( keypressed == '\x13')
		{

				APP_setBreak();
				setUImsg(UI_MSG_MENU);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_MENU;
		
		}
		break;



/*


		case UI_ADMIN_ACTIVITY:

		if( keypressed == '\x08')
		{
			if(ui.bufferIndex > 0 )
			{
				putCharToLcd(keypressed);
				ui.bufferIndex--;

			}
			else
			{
				setUImsg(UI_MSG_LINE);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_LINE;
			}
		}

/*
		else if( keypressed == '0')
		{
			setUImsg(UI_MSG_CLEAR_ISSUES);
			clearUIBuffer();
			clearUIInput();
			ui.state = UI_CLEAR_ISSUE;
		}


		break;

		case UI_PASSWORD:
		if( keypressed == '\x08')
		{
			if(ui.bufferIndex > 0 )
			{
				putCharToLcd(keypressed);
				ui.bufferIndex--;

			}
			else
			{
				setUImsg(UI_MSG_LINE);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_LINE;
			}

		}
		else if( keypressed == '\x0A')
		{
			
			ui.buffer[ui.bufferIndex] = '\0';
/*
			if( IAS_checkPassword(ui.buffer))
			{
				setUImsg(UI_MSG_ADMIN_ACTIVITY);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_ADMIN_ACTIVITY;
			}

			else
			{
				setUImsg(UI_MSG_LINE);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_LINE;
			}				

		}

		else 
		{
			ui.buffer[ui.bufferIndex] = keypressed;
			putCharToLcd('*');
			DelayMs(50);
			ui.bufferIndex++;
		}
		
		break;		
*/

		default:
		break;


	}
}

UINT8 mapKey(UINT8 scancode, UINT8 duration)
{
	UINT8 keypressed = 0xFF;
	switch(ui.state)
	{
		case UI_LINE:
			if( ui.bufferIndex < 1 )
			{

				if((scancode == 0x07)||(scancode == 0x03))
					keypressed = keyMap[scancode][0];
				else keypressed = 0xFF;
			}
			else 
			{
				if( (scancode == 0x0B)  || (scancode == 0x0f) )
					keypressed = keyMap[scancode][0];
				else keypressed = 0xFF;
			}

		
		break;

		case UI_MENU:
			if( ui.bufferIndex < 1 )
			{
				if((scancode == 0x03) || (scancode == 0x07) || (scancode == 0x0C)||(scancode == 0x0B) ) 					
						
						keypressed = keyMap[scancode][0];
				else
					keypressed = 0xFF;
			
			}

			else 
			{
				if( (scancode == 0x0B)  || (scancode == 0x0f) )
					keypressed = keyMap[scancode][0];
				else keypressed = 0xFF;
			}	
		
		break;

		case UI_REFERENCE:
			if( ui.bufferIndex < 1 )
			{
				if( (scancode == 0x03) || (scancode == 0x07) || (scancode == 0x0C)|| (scancode == 0x0E)
				|| (scancode == 0x0F))
				{
					keypressed = 0xFF;
					break;
				}
				else 	keypressed = keyMap[scancode][0];
			}
			else if( ui.bufferIndex < 24 )
			{

				if( (scancode == 0x03) || (scancode == 0x07) || (scancode == 0x0C)|| (scancode == 0x0F))
				
				{
					keypressed = 0xFF;
					break;
				}
				else 	keypressed = keyMap[scancode][0];

				if(scancode == ui.prevcode)
				{
					if(duration < MIN_KEYPRESS_DURATION )
					{
						ui.keyIndex++;
						if(ui.keyIndex == 5)
							ui.keyIndex = 0;
					}
					else
					{
						ui.keyIndex = 0;
					}
				}
				else
				{
					ui.keyIndex = 0;
		
				}

				keypressed = keyMap[scancode][ui.keyIndex];
			}

			else 
			{
			
				if( (scancode == 0x0B)  || (scancode == 0x0F))
					keypressed = keyMap[scancode][0];
				else keypressed = 0xFF;
			}


		break;

		case UI_OPERATORS:

			if( ui.bufferIndex < 1 )
			{
				if( (scancode == 0x03) || (scancode == 0x07) || (scancode == 0x0C)|| (scancode == 0x0E)
				|| (scancode == 0x0F))
				{
					keypressed = 0xFF;
					break;
				}
				else 	keypressed = keyMap[scancode][0];
			}
			else if( ui.bufferIndex < 2 )
			{

				if( (scancode == 0x03) || (scancode == 0x07) || (scancode == 0x0C)|| (scancode == 0x0F))
				
				{
					keypressed = 0xFF;
				}
				else keypressed = keyMap[scancode][0];
			}
				
			else 
			{
				if( (scancode == 0x0B)  || (scancode == 0x0E))
					keypressed = keyMap[scancode][0];
				else keypressed = 0xFF;
			}
		break;


	case UI_BREAK:
			keypressed = keyMap[scancode][0];
			if( (scancode != 0x0B) && (scancode != 0x0E))
				keypressed = 0xFF;
		break;



/*		case UI_CLEAR_ISSUE:

		keypressed = keyMap[scancode][0];

		if( (keypressed != '\x0A') && (keypressed != '\x08') )
		{
			keypressed = 0xFF;
		}

		break;

		case UI_ADMIN_ACTIVITY:
		keypressed = keypressed = keyMap[scancode][0];
		if( (keypressed != '0') && (keypressed !='\x08') && (keypressed != '\x0A') )
			keypressed = 0xFF;
		else 
		break;

		case UI_PASSWORD:
			keypressed = keyMap[scancode][0];
*/
		default:
		break;

	}

	return keypressed;
}

UINT8 getOperators()
{
	UINT8 i,station = 0;

	if( ui.bufferIndex == 1 )
	{
		ui.input[ui.inputIndex] = ui.buffer[0]-'0';
		ui.inputIndex++;
	}

	else if( ui.bufferIndex == 2 )
	{
		ui.input[ui.inputIndex] = 
			(ui.buffer[0] - '0')*10+(ui.buffer[1] -'0');
		ui.inputIndex++;
	}
	return station;
}


void getData()
{
	UINT8 i;

	for( i = 0; i< ui.bufferIndex; i++)
	{
		ui.input[ui.inputIndex] = ui.buffer[i];
		ui.inputIndex++;
		
	}
	ui.input[ui.inputIndex] = '\0';
	ui.inputIndex++;

	if( ui.inputIndex >= MAX_INPUT_CHARS )
		ui.inputIndex = 0;
}


void clearUIBuffer(void)
{
	memset(ui.buffer,0, MAX_INPUT_CHARS);
	ui.bufferIndex = 0;
	ui.keyIndex = 0;
	ui.prevcode = 0xFF;

}


void clearUIInput(void)
{
	memset((UINT8*)ui.input,0, MAX_INPUT_CHARS);
	ui.inputIndex = 0;
}



void setUImsg( UINT8 msgIndex )
{
	UINT8 i;

	const rom UINT8 *msg;

	msg = UI_MSG[msgIndex] ;

	clearLCD();

	i = 0;
	while( msg[i] != '\0')
	{
		putCharToLcd(msg[i]);
		i++;
	}
}


void putUImsg(UINT8 msgIndex)
{
	UINT8 i;

	const rom UINT8 *msg;

	msg = UI_MSG[msgIndex] ;

	i = 0;
	while( msg[i] != '\0')
	{
		putCharToLcd(msg[i]);
		i++;
	}
}


		