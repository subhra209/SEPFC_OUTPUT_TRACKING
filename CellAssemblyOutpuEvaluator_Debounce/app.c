#include "app.h"
#include "eep.h"
#include "rtc_driver.h"
#include "communication.h"
#include "mmd.h"
#include "string.h"
#include "linearkeypad.h"



typedef struct 
{
	UINT16 target;
	UINT32 tick;
}TargetInfo;


typedef struct 
{
	UINT16 target;
	UINT16 actual;
}KEInfo;




typedef enum
{
	NONE = 0,
	ACTIVE = 1,
	IN_TRANSITION = 2,
	IN_BREAK = 3


}LINE_STATE;
	

typedef struct _SHIFT
{
	UINT32 start;
	UINT32 end;
	UINT16 actual;

}SHIFT;

typedef struct
{
	UINT8 reference[MSG_MAX_CHARS+1];
	UINT16 cycleTime;
	UINT8 operators;

}LINECONFIG;


typedef struct _LINEINFO
{

	LINE_STATE state;
	LINE_STATE prevState;
	UINT8 reference[MSG_MAX_CHARS+1];


}LINEINFO;


typedef struct
{
	UINT8 currentShift;
	SCROLL_SPEED scrollSpeed;
	UINT16 shiftCumulative[3];
}APPINFO;


typedef struct _LINE
{
	LINE_STATE state;
	LINE_STATE prevState;
	UINT8 reference[MSG_MAX_CHARS+1];
	UINT16 cycleTime;
	UINT16 bottleNeckTime;
	UINT8 operators;
	UINT16 breakDuration;
	
	UINT8 targetHistoryIndex;


	UINT16 target;
	UINT16 htarget;		// i take hourly target
	UINT16 actual;
	BOOL lampState;
	UINT32 timeout;
	UINT8 getCycleTime;

	UINT16 hke;
	UINT16 ske;



	UINT8 keHistoryIndex;
	UINT8 kePrevHistoryIndex;
	
/*
	UINT8 changeHistory[20];

	UINT8 changeHistoryIndex;
	UINT16 transitionCount;
*/
}LINE;

enum
{
#ifdef TIME_DEBUG
	TIMEOUT = 65530,
#else
	TIMEOUT = 120,
#endif


	EEPROM_STATE = 0x00,
	EEPROM_PREV_STATE = EEPROM_STATE + sizeof( LINE_STATE),
	EEPROM_REFERENCE = EEPROM_PREV_STATE +MSG_MAX_CHARS* sizeof( LINE_STATE),
	EEPROM_CYCLE_TIME = EEPROM_REFERENCE+sizeof(UINT8),
	EEPROM_OPERATORS = EEPROM_CYCLE_TIME + sizeof( UINT16),
	EEPROM_BREAK_DURATION = EEPROM_OPERATORS + sizeof( UINT16),
	
	EEPROM_SHIFT = EEPROM_STATE + sizeof(LINEINFO)*2,

//	EEPROM_SCROLL_SPEED = EEPROM_SHIFT+sizeof(UINT8),
	EEPROM_SHIFT_CUMULATIVE = EEPROM_SCROLL_SPEED+3*sizeof(UINT16)


};



typedef struct _APP
{

	LINE line[2];
	UINT8 currentShift;
	SCROLL_SPEED scrollSpeed;
	UINT16 shiftCumulative[3];
	UINT32 tick;
	UINT8 prevMinute;
	UINT32 startTick;
	BOOL synchronized;

}APP;

enum
{
	ONE = 0,
	TWO = 1
};

const far rom UINT8* MESSAGE[] = { 	"BREAK   ",
									"        OPERATORS=", 
									"IN TRANSITION" };


#pragma idata app_data
APP app = {0} ;
LINECONFIG transitionInfo[2] = {0};
SHIFT shifts[3]={
{(UINT32)0,(UINT32)28800}, {(UINT32)28800,(UINT32)57600},{(UINT32)57600,(UINT32)0} 
};
#pragma idata

#pragma idata kehistory_data
KEInfo keHistory[2][60]={0};
#pragma idata 

#pragma idata targethistory_data
TargetInfo targetHistory[2][MAX_TRANSITIONS]={0};
#pragma idata 


MMD_Config mmdConfig= {0};


UINT8 readTimeDateBuffer[7] = {0};
UINT8 txBuffer[6] = {0};
extern UINT8 INPUT_FLAG[2];



UINT8 itoa(UINT8* buffer, INT16 data, INT8 index);
void setCycleTime(UINT8 line, UINT16 data );
void setActual(UINT8 line, UINT16 data );
void setTarget(UINT8 line, UINT16 data );
void setHTarget(UINT8 line, UINT16 data );
void setHourlyKe(UINT8 line, UINT16 data );
void setShiftKe(UINT8 line, UINT16 data );
void setShiftA(UINT8 line, UINT16 data );
void setShiftB(UINT8 line, UINT16 data );
void setShiftC(UINT8 line, UINT16 data );
UINT8 getCurrentShift( UINT32 tick);
UINT32 update_tick(INT32 hour,INT32 minute);
void shiftChange(UINT8);
void updateActual(UINT8 line);

void getCycleTime(UINT8 line, far UINT8* reference);
void updateTarget(UINT8 line);
void updateKE(UINT8 line);
void resetLamps(UINT8 line);
void copyStr( far UINT8*dst, const  far rom UINT8* src);
void updateEEPROM(UINT8 line);
void updateEEPROM_Break(UINT8 line);
void synchronize(void);
void updateLamps(UINT8 line);
UINT8 itoa_nr(far UINT8* buffer, INT16 data, INT8 index);
void stateChange_Break(UINT8 line);

void APP_init(void)
{
	UINT16 i,j;
	UINT8 data,speed,shift;
	far UINT8 *temp;
	UINT8 hour,minute,second;
	
	i = 0;
	for( i = 0; i < 2 ; i++)
	{
		app.line[i].state = app.line[i].prevState = 01;
		
		resetLamps(i);
		setCycleTime(i, app.line[i].cycleTime);
		DelayMs(10);
		setTarget(i,app.line[i].target);
		DelayMs(10);
		setActual(i,app.line[i].actual);
		DelayMs(10);
		setHourlyKe(i,app.line[i].hke);
		DelayMs(10);
		setShiftKe(i,app.line[i].ske);
		DelayMs(10);


		

		memset(app.line[i].reference,0x20,sizeof(app.line[i].reference));
		memset(transitionInfo[i].reference , 0x20 , sizeof(transitionInfo[i].reference));




		app.line[i].state = NONE;

	
		app.line[i].prevState = ACTIVE;


		app.line[i].keHistoryIndex = 59;
		app.line[i].kePrevHistoryIndex = 0;


	}
	setShiftA(0,shifts[0].actual);
	DelayMs(10);
	setShiftB(0,shifts[1].actual);
	DelayMs(10);
	setShiftC(1,shifts[2].actual);
	DelayMs(10);
	app.currentShift = 0xFF;


	app.scrollSpeed = 	5;
	
#ifdef TIME_DEBUG
//	app.synchronized = TRUE;
#endif

	

}

void APP_task(void)
{
	UINT8  i, j;
	BOOL refreshDisplay = FALSE;
	UINT8 shift ;
	UINT8 update = FALSE;
	UINT8 hour,minute,second = FALSE;
	UINT8 tempBuff[7] = { 0 };			 
  	
	hour = RTC_getHour();
	minute = RTC_getMinute();
	second = RTC_getSecond();

	app.tick = update_tick(6,0);

	shift = getCurrentShift(app.tick);

#ifdef __VERIFY__
	//Send RTC data on UART
	ReadRtcTimeAndDate( tempBuff );
	COM_txCMD_CHAN1( 0, 0xE1, tempBuff, 7 );
#endif
	if( app.prevMinute != minute)
	{
		update = TRUE;
		app.prevMinute = minute;
	}
		
	for( i = 0; i < 2 ; i++)
	{


		refreshDisplay = FALSE;
		switch( app.line[i].state )
		{
			case NONE:
			if( app.synchronized != TRUE )
			{
				synchronize();
				return;
			}
			if( update == TRUE)
			{


					itoa_nr(transitionInfo[i].reference, hour,2);
					transitionInfo[i].reference[2] =':';
					
					itoa_nr(&transitionInfo[i].reference[3], minute,2);
					transitionInfo[i].reference[i*8+5]= '\0';
	
					//MMD_clearSegment(i);
					mmdConfig.startAddress = i*8;
					mmdConfig.length = i*8+5;
					mmdConfig.symbolCount = strlen(transitionInfo[i].reference);
					mmdConfig.symbolBuffer = transitionInfo[i].reference;
					mmdConfig.scrollSpeed = 0;
					MMD_configSegment( i , &mmdConfig);	
					app.currentShift = shift;
				}


				
			

			break;

			case ACTIVE:
			{ 
						
				if( shift != app.currentShift )	//check for shift change
				{
					app.currentShift = shift;	//update current shift
					shiftChange(shift);				//handle shift change
					
					update = TRUE;
				
				}
			

				updateActual(i);
				updateTarget(i);

				if( update == TRUE )
				{
					++app.line[i].keHistoryIndex;
					if( app.line[i].keHistoryIndex >= 60 )
					{
						app.line[i].keHistoryIndex = 0;
					}
	
					++app.line[i].kePrevHistoryIndex;
					if( app.line[i].kePrevHistoryIndex >= 60 )
					{
						app.line[i].kePrevHistoryIndex = 0;
					}

			



				}	

				keHistory[i][app.line[i].keHistoryIndex].target =
						app.line[i].htarget;

				keHistory[i][app.line[i].keHistoryIndex].actual =
						app.line[i].actual;

				
				updateKE(i);

				updateLamps(i);
			}
			break;
			

			case IN_BREAK:		
				app.line[i].breakDuration++;
			break;

			case IN_TRANSITION:
			if( i == 0 ) //line 1
			{
				if( app.line[i].lampState == 0 )	
				{
					app.line[i].lampState = 1;
					RED_LINE_1 = 1;
					GREEN_LINE_1 = 1;
				}
				else if( app.line[i].lampState == 1 )	
				{
					app.line[i].lampState = 0;
					RED_LINE_1 = 0;
					GREEN_LINE_1 = 0;
				}		
	
			}

			else
			{
				if( app.line[i].lampState == 0 )	
				{
					app.line[i].lampState = 1;
					RED_LINE_2 = 1;
					GREEN_LINE_2 = 1;
				}
				else if( app.line[i].lampState == 1 )	
				{
					app.line[i].lampState = 0;
					RED_LINE_2 = 0;
					GREEN_LINE_2 = 0;
				}	
			}	

			app.line[i].timeout--;
			if( app.line[i].timeout == 0)
			{
				app.line[i].state = app.line[i].prevState;
				resetLamps(i);
				if( app.line[i].prevState == IN_BREAK)
				{
					stateChange_Break(i);
				}
				else
				{
					MMD_clearSegment(i);
					mmdConfig.startAddress = i*8;
					mmdConfig.length = i*8+8;
					mmdConfig.symbolCount = strlen(app.line[i].reference);
					mmdConfig.symbolBuffer = app.line[i].reference;
					mmdConfig.scrollSpeed = app.scrollSpeed;
					MMD_configSegment( i , &mmdConfig);	
					setCycleTime(i, app.line[i].cycleTime );
					DelayMs(10);
					app.line[i].getCycleTime = FALSE;
				}
				
				

			}
			break;

			default:
			break;
		}
	
	}



	
}


UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket)
{
	UINT8 i, j;
	UINT8 rxCode = rxPacket[0];
	UINT8 length = 0;


	switch( rxCode)
	{
		case CMD_SET_REFERENCE:
		{
			UINT8 line = rxPacket[1] - 2;
			if( app.line[line].state == IN_TRANSITION && (app.line[line].getCycleTime == FALSE))
			{
				
				
				memset(transitionInfo[line].reference,0x20,sizeof(transitionInfo[line].reference));

				MMD_clearSegment(line);
				mmdConfig.startAddress = 8*line+0;
				mmdConfig.length = line*8+8;
				mmdConfig.symbolCount = 8;
				mmdConfig.symbolBuffer = transitionInfo[line].reference;
				mmdConfig.scrollSpeed = 0;
				MMD_configSegment( line , &mmdConfig);

				strcpy( transitionInfo[line].reference , &rxPacket[2]);
				setCycleTime(line, 0 );
		
				getCycleTime(line , transitionInfo[line].reference);
				app.line[line].getCycleTime = TRUE;
				*txCode = CMD_SET_REFERENCE;
				length = 0;
			}
		}
		break;

		case CMD_SET_OPERATORS:
		{
			UINT8 line = rxPacket[1] - 2;
			UINT8 msgLength;
			if( app.line[line].state == IN_TRANSITION)
			{
				
				app.line[line].operators = rxPacket[2];
				*txCode = CMD_SET_OPERATORS;
				length = 0;
				targetHistory[line][app.line[line].targetHistoryIndex].target 
							= app.line[line].target;										//STORE TARGET & TICK in history

				targetHistory[line][app.line[line].targetHistoryIndex].tick 
							= app.tick;	
				
				app.line[line].targetHistoryIndex++;
				app.line[line].breakDuration = 0;				//reset break for current session									
				app.line[line].state = app.line[line].prevState;

				msgLength = strlen(app.line[line].reference);
				itoa_nr(&app.line[line].reference[msgLength - 9],app.line[line].operators,2);
			}
		}				

		break;

		case CMD_SET_BREAK:
		{
			UINT8 line = rxPacket[1] - 2;
			if( app.line[line].prevState == ACTIVE)
			{
				app.line[line].prevState = IN_BREAK;					
				app.line[line].state = IN_BREAK;
				stateChange_Break(line);
				*txCode = CMD_SET_BREAK;
				length = 0;
				resetLamps(line);
				setCycleTime(line, -1 );
			}

			else if( app.line[line].prevState == IN_BREAK)
			{
				app.line[line].state = ACTIVE;
				app.line[line].prevState = ACTIVE;

				MMD_clearSegment(line);

				mmdConfig.startAddress = line*8;
				mmdConfig.length = line*8+8;
				mmdConfig.symbolCount = strlen(app.line[line].reference);
				mmdConfig.symbolBuffer = app.line[line].reference;
				mmdConfig.scrollSpeed = app.scrollSpeed;
				MMD_configSegment( line , &mmdConfig);			//configure mmd segment
				*txCode = CMD_SET_BREAK;
				length = 0;										//configure mmd segment
				setCycleTime(line,app.line[line].cycleTime);
				resetLamps(line);
			}
		}
			

		break;


		case CMD_SET_CYCLE_TIME:
		{
			UINT8 line = rxPacket[1] - 2;
			UINT8 msgLength = 0;
			if( app.line[line].state == IN_TRANSITION && (app.line[line].getCycleTime == TRUE))
			{
				app.line[line].cycleTime = ((UINT16)rxPacket[3]*256) + rxPacket[2];
				app.line[line].bottleNeckTime = ((UINT16)rxPacket[5]*256) + rxPacket[4];

				msgLength = strlen(transitionInfo[line].reference);
				transitionInfo[line].reference[msgLength] = 0x20;
				transitionInfo[line].reference[msgLength + 5 ] = '\0';
				strcpy(app.line[line].reference , transitionInfo[line].reference);		
				
				msgLength = strlen(app.line[line].reference);
				copyStr(&app.line[line].reference[msgLength], MESSAGE[1]);
				
				msgLength = strlen(app.line[line].reference);
				itoa_nr(&app.line[line].reference[msgLength],app.line[line].operators,2);

				memset(&(app.line[line].reference[msgLength+2]),0x20,7);
				app.line[line].reference[msgLength+9] = '\0';
				
				
				if( app.line[line].prevState == ACTIVE )
				{
					
					MMD_clearSegment(line);
					mmdConfig.startAddress = line*8;
					mmdConfig.length = line*8+8;
					mmdConfig.symbolCount = strlen(app.line[line].reference);
					mmdConfig.symbolBuffer = app.line[line].reference;
					mmdConfig.scrollSpeed = app.scrollSpeed;
					MMD_configSegment( line , &mmdConfig);			//configure mmd segment
					setCycleTime( line, app.line[line].cycleTime );		
				}
				else if( app.line[line].prevState == IN_BREAK )
				{
					stateChange_Break(line);
				}

				*txCode = CMD_SET_CYCLE_TIME;
				length = 0;

				app.line[line].state = app.line[line].prevState; 
				targetHistory[line][app.line[line].targetHistoryIndex].target 
						= app.line[line].target;										//STORE TARGET & TICK in history

				targetHistory[line][app.line[line].targetHistoryIndex].tick 
							= app.tick;	
				
				app.line[line].targetHistoryIndex++;
				

				app.line[line].getCycleTime = FALSE;
				app.line[line].breakDuration = 0;				//reset break for current session								
				
				resetLamps(line);
			}
			resetLamps(line);
		}
				
		break;

		case CMD_START_TRANSITION:
		{
			UINT8 line = rxPacket[1] - 2;
			//configure mmd segment
			*txCode = CMD_START_TRANSITION;
			length = 0;
			
			if( app.line[line].state != NONE && (app.line[line].state != IN_TRANSITION) )
				app.line[line].prevState = app.line[line].state;
			app.line[line].state = IN_TRANSITION ;
			app.line[line].timeout = TIMEOUT;
			
		}
		break;
		
		case CMD_SYNCHRONIZE:
		{
				WriteRtcTimeAndDate(&rxPacket[1]);
				
				app.synchronized = TRUE;
				*txCode = CMD_SYNCHRONIZE;
				length = 0;
		}


		break;
	


		case CMD_PING:
		*txCode = CMD_PING;
		length = 0;
		break;

		default: break;
	}
	return length;


}


void stateChange_Break(UINT8 line)
{
		copyStr( transitionInfo[line].reference , MESSAGE[0] );
		MMD_clearSegment(line);
		mmdConfig.startAddress = line*8;
		mmdConfig.length = line*8+8;
		mmdConfig.symbolCount = strlen(transitionInfo[line].reference);
		mmdConfig.symbolBuffer = transitionInfo[line].reference;
		mmdConfig.scrollSpeed = app.scrollSpeed;
		MMD_configSegment( line , &mmdConfig);			//configure mmd segment
		setCycleTime( line, -1 );
}



void updateActual(UINT8 line)
{
       if(INPUT_FLAG[line] == 1)
        {
					app.line[line].actual++;
					shifts[app.currentShift-1].actual++;
					setActual( line, app.line[line].actual );
					DelayMs(10);
					if( (app.currentShift == 1) )
						setShiftA(0, shifts[app.currentShift-1].actual);
					else if( (app.currentShift == 2))
						setShiftB(0,shifts[app.currentShift-1].actual);
					else if( (app.currentShift == 3))
						setShiftC(1,shifts[app.currentShift-1].actual);
					DelayMs(10);
        		INPUT_FLAG[line] = 0;
			}
			
  	
             }
    

void updateTarget(UINT8 line)
{
	UINT8 i;
	INT32 temp,temp1,prevTick;		// i take hprevTick
	UINT16 currentTarget =0;
	UINT16 hcurrentTarget =0;	
	INT32 hprevTick;
	INT32 tick = (INT32)app.tick;
 



	if( app.line[line].cycleTime == 0 || (app.line[line].bottleNeckTime == 0 ))
	{
		app.line[line].target = 0;
		app.line[line].htarget = 0;			//i add this line
	}

	else
	{

		if( app.line[line].targetHistoryIndex > 0 )
		{
			prevTick = 
			targetHistory[line][app.line[line].targetHistoryIndex-1].tick;
			currentTarget = targetHistory[line][app.line[line].targetHistoryIndex-1].target;
			hcurrentTarget = keHistory[line][app.line[line].kePrevHistoryIndex].target;
		}
		else 
		{
			prevTick = shifts[app.currentShift-1].start;
			
		}
		hprevTick = (tick - 3600);
		if ( hprevTick < prevTick) // get time from rtc and subtract 3600 
		{ 
			hprevTick = prevTick;
			hcurrentTarget = currentTarget;
		}
		
		

#ifdef __ERROR_DEBUG__

		if( line == 1)	
		{
			COM_txCMD_CHAN1(line, 0xE0, &app.tick,4);
			COM_txCMD_CHAN1(line, 0xE1, &prevTick,4);
		}
#endif	


		if( tick > prevTick )	
			temp = tick - prevTick - app.line[line].breakDuration;
		else
			temp = prevTick - tick - app.line[line].breakDuration;
		if( temp <  0 ) temp =0;
		temp1 = temp;
		temp *= app.line[line].operators;
		temp /= app.line[line].cycleTime;

		temp1 /= app.line[line].bottleNeckTime;

		if( temp > temp1 )
			temp = temp1;

			
		currentTarget +=temp;

		if( currentTarget != app.line[line].target )
		{

			app.line[line].target = currentTarget;
			setTarget(line,app.line[line].target);
			DelayMs(10);
		}

// for hourly traget calculate

		if( tick > hprevTick )	
			temp = tick - hprevTick - app.line[line].breakDuration;
		else
			temp = hprevTick - tick - app.line[line].breakDuration;
		if( temp <  0 ) temp =0;
		temp1 = temp;
		temp *= app.line[line].operators;
		temp /= app.line[line].cycleTime;

		temp1 /= app.line[line].bottleNeckTime;

		if( temp > temp1 )
			temp = temp1;

			
		hcurrentTarget += temp;

		if( hcurrentTarget != app.line[line].htarget )
		{

			app.line[line].htarget = hcurrentTarget;
			setHTarget(line,app.line[line].htarget);
			DelayMs(10);
		}
	
	}

}


void updateKE(UINT8 line)
{
	UINT32 temp; 
	UINT32 ske ;
	UINT32 hke =0;
	UINT32 temp2;
	
	if( app.line[line].target == 0 )
		ske = 0;
	else 
	{
		temp =((UINT32)app.line[line].actual * 100*100 )
				/app.line[line].target;
		
		if(temp >= 10000 )
		{
			ske = 999;
		}
		else 
			ske = temp/10;
	}

	if( ske != app.line[line].ske )
	{
		app.line[line].ske = ske;
		setShiftKe(line, app.line[line].ske);
		DelayMs(10);
	}

	temp = 
	((UINT32)	keHistory[line][app.line[line].keHistoryIndex].actual
		 - (UINT32)keHistory[line][app.line[line].kePrevHistoryIndex].actual )*100*100;

	temp2 = (keHistory[line][app.line[line].keHistoryIndex].target
			-keHistory[line][app.line[line].kePrevHistoryIndex].target);

	if( temp2 == 0 )
	{	hke = 0 ;
	}
	else 
	{
		temp= (temp)/temp2; 
	
		if(temp >= 10000 )
		{
			hke = 999;
		}
		else 
			hke = temp/10;
	}
	if( hke != app.line[line].hke )
	{
		app.line[line].hke = hke;
		setHourlyKe(line, app.line[line].hke);
		DelayMs(10);
	}
}

void getCycleTime(UINT8 line, UINT8* reference)
{
	UINT8 length = strlen(reference);
	COM_txCMD_CHAN1(line, 	CMD_GET_CYCLE_TIME , reference,length+1);
	
}


void synchronize()
{
	
	COM_txCMD_CHAN1(0x0, CMD_SYNCHRONIZE ,NULL, 0);
	
}


void shiftChange(UINT8 shift)
{
	UINT8 i,j;
	//app.currentShift = shift;
	for( i = 0; i < 2 ; i++)
	{
		app.line[i].target = 0;
		app.line[i].actual = 0;
		app.line[i].ske = 0;
		app.line[i].hke = 0;
		app.line[i].breakDuration = 0;
		
		for(j = 0; j < MAX_TRANSITIONS ; j++)
		{
			targetHistory[i][j].target = 0;
			targetHistory[i][j].tick = 0;
		} 
		app.line[i].targetHistoryIndex = 0;

		for(j = 0; j < 60 ; j++)
		{
			keHistory[i][j].target = 0;
			keHistory[i][j].actual = 0;
		} 
		app.line[i].keHistoryIndex = 59;
		app.line[i].kePrevHistoryIndex = 0;
	
		setTarget(i, app.line[i].target);
		DelayMs(10);
		setActual(i, app.line[i].actual);
		DelayMs(10);

		setHourlyKe(i, app.line[i].ske);
		DelayMs(10);
		setShiftKe(i, app.line[i].hke);
		DelayMs(10);

	}
	shifts[shift-1].actual = 0;
	if( app.currentShift == 1  )
	{
			setShiftA(i, shifts[app.currentShift-1].actual);
		DelayMs(10);
	}
	else if( app.currentShift == 2 )
	{	
		setShiftB(i,shifts[app.currentShift-1].actual);
		DelayMs(10);
	}
	else
	{	
		setShiftC(i,shifts[app.currentShift-1].actual);
		DelayMs(10);
	}
}

UINT32 update_tick(INT32 hour, INT32 minute)
{
	INT32 temp;
	temp = (RTC_getSecondCount()-(hour*60*60 + minute*60));

	
	if(temp < 0 )
		temp = temp+86400;  		//24*60*60
	else
		temp = temp;

	return (UINT32)temp;
}


UINT8 getCurrentShift(UINT32 tick)
{

	 if( 	tick < shifts[0].end)
		return 1;
	else if( 	tick < shifts[1].end )
		return 2;
	else
		 return 3;

}


UINT8 itoa(UINT8* buffer, INT16 data, INT8 index)
{
	INT8 i;
	
	if( data < 0 )
	{
		for(i = 0; i< index ; i++)
		{
			*(buffer+i) = ' ';
		}
		return index;
	}

	else if( data == 0 )
	{
		for(i = 0; i< index ; i++)
		{
			*(buffer+i) = '0';
		}
		return index;
	}
	else
	{
			
		i = index;
		for( --i; i >=0 ; --i)
		{
			if( data == 0 )
				*(buffer+i) = ' ';
			else 
			*(buffer + i) = data%10 + '0';
			data/= 10;
		}	
	}
	return index ;
}


UINT8 itoa_nr(UINT8* buffer, INT16 data, INT8 index)
{
	INT8 i;
	
	if( data < 0 )
	{
		for(i = 0; i< index ; i++)
		{
			*(buffer+i) = '0';
		}
		return index;
	}

	else if( data == 0 )
	{
		for(i = 0; i< index ; i++)
		{
			*(buffer+i) = '0';
		}
		return index;
	}
	else
	{
			
		i = index;
		for( --i; i >=0 ; --i)
		{
			if( data == 0 )
				*(buffer+i) = '0';
			else 
			*(buffer + i) = data%10 + '0';
			data/= 10;
		}	
	}
	return index ;
}


//SET CYCLE TIME
void setCycleTime(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[4] = {0};
	UINT8 length;
	length = itoa( buffer , data, 4 );


	COM_txCMD_CHAN1(line, 	CMD_UPDATE_CYCLE_TIME , buffer,4);
} 


//SET ACTUAL
void setActual(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(line, 	CMD_UPDATE_ACTUAL , buffer,3);
} 

//SET TARGET
void setTarget(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(line, 	CMD_UPDATE_TARGET , buffer, 3);
} 

void setHTarget(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(line, 	CMD_H_TARGET , buffer, 3);
} 

//SET HOURLY KE
void setHourlyKe(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(line, CMD_UPDATE_HOURLY_KE , buffer, 3);
} 


//SET SHIFT KE
void setShiftKe(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(line, CMD_UPDATE_SHIFT_KE , buffer, 3);
} 

//SET SHIFT A
void setShiftA(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(0, CMD_UPDATE_SHIFT_A , buffer, 3);
} 

//SET SHIFT B
void setShiftB(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(0, CMD_UPDATE_SHIFT_B , buffer, 3);
}

void setShiftC(UINT8 line, UINT16 data )
{
	UINT8 i;
	UINT8 buffer[3] = {0};
	UINT8 length;
	length = itoa( buffer , data, 3 );


	COM_txCMD_CHAN1(1, CMD_UPDATE_SHIFT_A , buffer, 3);
}


void resetLamps(UINT8 line)
{
	if( line == 0)
	{
		RED_LINE_1 = 0;
		GREEN_LINE_1 = 0;
	}
	else if( line == 1 )
	{
		
		RED_LINE_2 = 0;
		GREEN_LINE_2 = 0;
	}
	app.line[line].lampState = 0;
}


void updateLamps(UINT8 line)
{
	if(line == 0 )
	{
		if( app.line[line].hke >=800 )
		{
			RED_LINE_1 = 0;
			GREEN_LINE_1 = 1;
		}
		else 
		{
		
			RED_LINE_1 = 1;
			GREEN_LINE_1 = 0;
		}
	}
	else
	{
		if( app.line[line].hke >=800 )
		{
			RED_LINE_2 = 0;
			GREEN_LINE_2 = 1;
		}
		else 
		{
		
			RED_LINE_2 = 1;
			GREEN_LINE_2 = 0;
		}
	}
		
}




void copyStr(far UINT8*dst, const far rom UINT8* src)
{
	while( 	*dst++ = *src++)
	{
		;
	}
}

void updateEEPROM(UINT8 line)
{
	UINT8 i;
	UINT16 aptr = EEPROM_STATE + line*sizeof(LINEINFO);
	UINT8* dptr = (UINT8*)&app.line[line];
	for(i = 0; i < sizeof( LINEINFO ) ; i++)
	{
		Busy_eep();
		Write_b_eep(aptr++,*dptr++);
		Busy_eep();
	}
	aptr = EEPROM_SHIFT;
	dptr = &(app.currentShift);
	for(i = 0; i < sizeof(APPINFO ) ; i++)
	{
		Busy_eep();
		Write_b_eep(aptr++,*dptr++);
		Busy_eep();
	}
}

void updateEEPROM_Break(UINT8 line)
{
	UINT8 i;
	UINT16 aptr = EEPROM_BREAK_DURATION;
	UINT8* dptr = (UINT8*)&app.line[line].breakDuration;
	for(i = 0; i < sizeof( UINT16 ) ; i++)
	{
		Busy_eep();
		Write_b_eep(aptr++,*dptr++);
		Busy_eep();
	}
}


/*void updateActual(UINT8 line)
{
	if( LinearKeyPad_getKeyState(line) == KEY_PRESSED)
	{
		app.line[line].actual++;
		shifts[app.currentShift-1].actual++;
		setActual( line, app.line[line].actual );
		DelayMs(10);
		if( (app.currentShift == 1) )
			setShiftA(0, shifts[app.currentShift-1].actual);
		else if( (app.currentShift == 2))
			setShiftB(0,shifts[app.currentShift-1].actual);
		else if( (app.currentShift == 3))
			setShiftC(1,shifts[app.currentShift-1].actual);
		DelayMs(10);
		
	}	
}
*/