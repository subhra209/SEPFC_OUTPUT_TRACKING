#include "mmd.h"
#include "communication.h"
#include "board.h"
#include "config.h"
#include "uart_driver.h"
#include "linearkeypad.h"



#define EEPROM_SCROLL_SPEED (0)
#define EEPROM_MSG (EEPROM_SCROLL_SPEED+ 1)

enum
{
	CMD_PING = 0xA0,

	CMD_UPDATE_CYCLE_TIME = 0xB0,
	CMD_UPDATE_TARGET = 0xB1,
	CMD_UPDATE_ACTUAL = 0xB2,
	CMD_UPDATE_HOURLY_KE = 0xB3,
	CMD_UPDATE_SHIFT_KE = 0xB4,
	CMD_UPDATE_SHIFT_A = 0xB5,
	CMD_UPDATE_SHIFT_B = 0xB6,
	CMD_BLINK_CYLCE_TIME = 0xB7,
	CMD_BLINK_TARGET = 0xB8,
	CMD_H_TARGET = 0XB9,

	CMD_START_TRANSITION = 0x70,
	CMD_SET_REFERENCE = 0x77,
	CMD_SET_OPERATORS = 0x72,
	CMD_SET_BREAK = 0x73,
	CMD_SET_CYCLE_TIME = 0x74,

	CMD_SYNCHRONIZE = 0x75,

	CMD_GET_CYCLE_TIME = 0x76
	
};

enum
{ 
  ExINT0,
  ExINT1
};

void APP_init(void);
void APP_task(void);
UINT8 APP_comCallBack( far UINT8 *rxPacket,  far UINT8* txCode, far UINT8** txPacket);
