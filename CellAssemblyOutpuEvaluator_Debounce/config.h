#ifndef CONFIG_H
#define CONFIG_H

/*
*------------------------------------------------------------------------------
* config.h
*
*/


#define DEVICE_ADDRESS			0x01


#define __FACTORY_CONFIGURATION__

#define __DISPLAY_TEST__ 

//MMD module configuration
#define MMD_MAX_CHARS		16
#define MMD_MAX_SEGMENTS	2
#define MMD_MAX_ADDRESS		128
//#define __MMD_STATIC__


//COM module configuration
//#define __NO_CHECKSUM__
#define __BCC_XOR__
#define __RESPONSE_ENABLED__
//#define __LOOP_BACK__
#define BROADCAST_ADDRESS		0xFF
#define CMD_SOP	0xAA
#define CMD_EOP 0xBB
#define RESP_SOP	0xCC
#define RESP_EOP	0xDD


enum
{
	CMD_PACKET_SIZE = 30,
	RESP_PACKET_SIZE = 30
};

#define 	RX_PACKET_SIZE		(60)	
#define 	TX_PACKET_SIZE		(60)


//KEYPAD CONFIGURATION
//#define __SIMULATION__


// Enable for external eeprom access
// Comment out this line if internal eeprom access required
#define EEP_EXTERRNAL
// For uart enable
//#define EN_UART
// Enable for hardware i2c module usage
//#define USE_MSSP_I2C


//RTC CONFIGURATION
#define TIME_DEBUG

//#define RTC_DS1307
//#define RTC_DS3232

//#define __SET_RTC__


//APPLICATION CONFIGURATION
#define MSG_MAX_CHARS 60
#define MAX_TRANSITIONS 20

//#define __ERROR_DEBUG__

/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

#endif
/*
*  End of config.h
*/



