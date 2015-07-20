

/*
*------------------------------------------------------------------------------
* device.h
*

#ifndef __DEVICE__
#define __DEVICE__
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <p18f4520.h>
#include "typedefs.h"
#include <delays.h>
//#include <eep.h>
#include "config.h"

/*
*------------------------------------------------------------------------------
* Hardware Port Allocation
*------------------------------------------------------------------------------
*/

/*
#define	KBD_ROW3				PORTBbits.RB0	
#define	KBD_ROW3_DIR			TRISBbits.TRISB0
#define	KBD_ROW2				PORTBbits.RB1
#define	KBD_ROW2_DIR			TRISBbits.TRISB1	
#define	KBD_ROW1				PORTBbits.RB2
#define	KBD_ROW1_DIR			TRISBbits.TRISB2
#define	KBD_ROW0				PORTBbits.RB3
#define	KBD_ROW0_DIR			TRISBbits.TRISB3
#define	KBD_COL0				PORTBbits.RB4
#define	KBD_COL0_DIR			TRISBbits.TRISB4	
#define	KBD_COL1				PORTBbits.RB5
#define	KBD_COL1_DIR			TRISBbits.TRISB5	
#define	KBD_COL2				PORTBbits.RB6
#define	KBD_COL2_DIR			TRISBbits.TRISB6	
#define	KBD_COL3				PORTBbits.RB7
#define	KBD_COL3_DIR			TRISBbits.TRISB7

*/	
/*
#define	KBD_ROW2				PORTAbits.RA2	
#define	KBD_ROW2_DIR			TRISAbits.TRISA2	
#define	KBD_ROW3				PORTAbits.RA3
#define	KBD_ROW3_DIR			TRISAbits.TRISA3
*/


/* Keypad Association */
#define		KEYPAD_PORT				PORTB
#define		KEYPAD_PORT_DIR			TRISB
#define		KEYPAD_DEC_INT			PORTBbits.RB7		
#define		KEYPAD_DEC_INT_DIR		TRISBbits.TRISB7
#define		KEYPAD_BCD3				PORTBbits.RB3		
#define		KEYPAD_BCD3_DIR			TRISBbits.TRISB3
#define		KEYPAD_BCD2				PORTBbits.RB4		
#define		KEYPAD_BCD2_DIR			TRISBbits.TRISB4
#define		KEYPAD_BCD1				PORTBbits.RB2		
#define		KEYPAD_BCD2_DI1			TRISBbits.TRISB2
#define		KEYPAD_BCD0				PORTBbits.RB5		
#define		KEYPAD_BCD2_DI0			TRISBbits.TRISB5

// Heart Beat 
#define	HEART_BEAT				LATEbits.LATE0
#define	HEART_BEAT_DIR			TRISEbits.TRISE0				


// Buzzer O/P
#define	BUZ_OP					LATAbits.LATA3
#define	BUZ_OP_DIR				TRISAbits.TRISA3	


#define LAMP_RED				LATAbits.LATA0
#define LAMP_RED_DIR			TRISAbits.TRISA0

#define LAMP_YELLOW				LATAbits.LATA1
#define LAMP_YELLOW_DIR			TRISAbits.TRISA1

#define LAMP_GREEN				LATAbits.LATA2
#define LAMP_GREEN_DIR			TRISAbits.TRISA2


// LCD Port
#define LCD_DAT4_PORT			LATD
#define	LCD_D7					LATDbits.LATD7 	
#define	LCD_D7_PIN				PORTDbits.RD7	
#define	LCD_D7_DIR				TRISDbits.TRISD7				
#define	LCD_D6					LATDbits.LATD6	
#define	LCD_D6_DIR				TRISDbits.TRISD6
#define	LCD_D5					LATDbits.LATD5	
#define	LCD_D5_DIR				TRISDbits.TRISD5
#define	LCD_D4					LATDbits.LATD4	
#define	LCD_D4_DIR				TRISDbits.TRISD4		
#define	LCD_E					LATDbits.LATD3	
#define	LCD_E_DIR				TRISDbits.TRISD3			
#define	LCD_RW					LATDbits.LATD2	
#define	LCD_RW_DIR				TRISDbits.TRISD2
#define	LCD_RS					LATDbits.LATD1	
#define	LCD_RS_DIR				TRISDbits.TRISD1

// Rs485 / RS232 Serial commnunicaton port
#define		TX_EN					PORTCbits.RC0			// TX control for RS485 communication
#define		TX_EN_DIR				TRISCbits.TRISC0
#define 	SER_TX					PORTCbits.RC6   		// serial transmit
#define		SER_TX_DIR				TRISCbits.TRISC6
#define 	SER_RX					PORTCbits.RC7			// serial receive
#define		SER_RX_DIR				TRISCbits.TRISC7


/*
*------------------------------------------------------------------------------
* Public Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Macros
*------------------------------------------------------------------------------
*/


// Direction controle bit is processor specific ,
#define PORT_OUT				(BOOL)(0)
#define PORT_IN					(BOOL)(1)

#define SWITCH_OFF				(BOOL)(0)
#define SWITCH_ON				(BOOL)(1)

#define HOOTER_OFF				(BOOL)(1)
#define HOOTER_ON				(BOOL)(0)


#define GetSystemClock()		(16000000ul)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)
#define GetPeripheralClock()	GetInstructionClock()




#define TICK_PERIOD	(65535- 20000)


#define BUZZER_ON()			BUZ_OP = SWITCH_ON;
#define BUZZER_OFF()		BUZ_OP = SWITCH_OFF;

// Disable global interrupt bit.
#define ENTER_CRITICAL_SECTION()	INTCONbits.GIE = 0;INTCONbits.PEIE = 0;

#define DISABLE_UART_TX_INTERRUPT()	PIE1bits.TXIE = 0
#define ENABLE_UART_TX_INTERRUPT()	PIE1bits.TXIE = 1

#define DISABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 0
#define ENABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 1


#define DISABLE_TMR0_INTERRUPT()	INTCONbits.TMR0IE=0
#define ENABLE_TMR0_INTERRUPT()		INTCONbits.TMR0IE=1


#define LCD_DATA_PORT_IN()	TRISDbits.TRISD7 = 1;\
							TRISDbits.TRISD6 = 1;\
							TRISDbits.TRISD5 = 1;\
							TRISDbits.TRISD4 = 1;\
							TRISDbits.TRISD1 = 0;\
							TRISDbits.TRISD2 = 0;\
							TRISDbits.TRISD3 = 0;

#define LCD_DATA_PORT_OUT()	TRISDbits.TRISD7 = 0;\
							TRISDbits.TRISD6 = 0;\
							TRISDbits.TRISD5 = 0;\
							TRISDbits.TRISD4 = 0;\
							TRISDbits.TRISD1 = 0;\
							TRISDbits.TRISD2 = 0;\
							TRISDbits.TRISD3 = 0;


// Enable global interrupt bit.
#define EXIT_CRITICAL_SECTION()		INTCONbits.GIE = 1;INTCONbits.PEIE = 1;

#define ENABLE_GLOBAL_INT()			EXIT_CRITICAL_SECTION()


#define ENB_485_TX()	TX_EN = 1;
#define ENB_485_RX()	TX_EN = 0

#define Delay10us(us)		Delay10TCYx(((GetInstructionClock()/1000000)*(us)))
#define DelayMs(ms)												\
	do																\
	{																\
		unsigned int _iTemp = (ms); 								\
		while(_iTemp--)												\
			Delay1KTCYx((GetInstructionClock()+999999)/1000000);	\
	} while(0)

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

extern void InitializeBoard(void);
extern void InitializeBeatLed(void);
extern void InitializeBuzzer(void);
extern rom void UpdateHeartBeatLedTask(void);
extern rom void UpdateBuzzerOutPutTask(void);
extern void EnableBuzzer(UINT16 interval, UINT8 numOfTimes);
extern UINT8 ConvertBCD2HEX(UINT16 bcd);
extern UINT16 BCD2HEX(UINT8 val);
extern UINT8 HEX2BCD(UINT8 hexIn);
extern void ConvertAndDisplay16BitValue(UINT16 inValue, UINT8 *pNewVal);

/*
*  End of device.h
*/
