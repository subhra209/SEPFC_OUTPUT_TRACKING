#ifndef _APC_IAS_H_
#define _APC_IAS_H_


/*
*----------------------------------------------------------------------------------------------------------------
*	MACROS
*-----------------------------------------------------------------------------------------------------------------
*/

//#define __FACTORY_CONFIGURATION__


/*
*----------------------------------------------------------------------------------------------------------------
*	Enumerations
*-----------------------------------------------------------------------------------------------------------------
*/
typedef enum 
{
	OFF,
	ON
}INDICATOR_STATE;

typedef enum _ISSUE_TYPE
{
	NO_ISSUE,
	RAISED,
	RESOLVED
}ISSUE_TYPE;



typedef enum _IAS_PARAM
{
	MAX_KEYPAD_ENTRIES = 32,
	MAX_ISSUES = 6,
	MAX_DEPARTMENTS = 20,
	MAX_LOG_ENTRIES = 7,
	LOG_BUFF_SIZE = 32+1

}IAS_PARAM;



typedef enum _LOGDATA
{
	HW_TMEOUT = 10,
	APP_TIMEOUT = 1000,
	TIMESTAMP_UPDATE_VALUE = (APP_TIMEOUT/HW_TMEOUT)
}LOGDATA;

typedef enum
{
	ISSUE_RESOLVED,
	ISSUE_RAISED,
	ISSUE_CRITICAL
}IAS_STATE;

enum
{
	CMD_START_TRANSITION = 0x70,
	CMD_SET_REFERENCE_CODE = 0x71,
	CMD_SET_OPERATORS = 0x72,
	CMD_SET_BREAK = 0x73

};



extern void APP_init(void);
extern void APP_task(void);
extern BOOL  IAS_updateIssueInfo( UINT8 depId , ISSUE_TYPE issueType);
extern void IAS_updateIssues(UINT8* data);
extern void IAS_acknowledgeIssues(void);
void IAS_clearIssues(void);
BOOL IAS_checkPassword(UINT8 *password);
void APP_startTransition(UINT8*data);
void APP_setReference(UINT8*data, UINT8 length);
void APP_setOperators(UINT8*data, UINT8 length);
void APP_setBreak(void);
#endif