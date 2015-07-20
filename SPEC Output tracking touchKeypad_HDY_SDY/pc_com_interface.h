
#ifndef PC_COM_INTERFACE_H
#define PC_COM_INTERFACE_H


/*
*------------------------------------------------------------------------------
* pc_com_interface.h
*
* Include file for pc_com_interface module.
*
* (C)2010 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: pc_com_interface.h
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 22/04/2010
*------------------------------------------------------------------------------
*
* Revision 0.0 22/04/2010 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "typedefs.h"
#include "uart_driver.h"

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
// Enable this macro to enable BCC check in pc communication
#define 	COMM_CHECK_BCC

#define OFFSET_DEV_ID		(0)
#define OFFSET_CMD			(2)
#define OFFSET_SIZE			(4)
#define OFFSET_DATA			(6)

#define 	OFFSET_BCC(PACKET)		(DATALENGTH + PACKET[2] + 1)
//Offset of ETX in PacketBuf.
#define 	OFFSET_ETX(PACKET)		(OFFSET_BCC(PACKET) + 1)
//Total Bytes Length in PacketBuf.
#define 	PACKET_LEN(PACKET)		(OFFSET_ETX(PACKET) + 1)
//CheckSum PacketBuf into PacketBuf[OFFSET_BCC(PacketBuf)].
#define 	PACKET_BCC(PACKET)\
	do\
	{int i = 1, bcc = 0;\
	for(; i<OFFSET_BCC(PACKET); i++)\
		bcc ^= PACKET[i];PACKET[OFFSET_BCC(PACKET)] = bcc;\
	} while(0);\

#define 	SOF							(0xAA) 	// Start of frame
#define 	EOF							(0xBB)	// End of frame

#define 	RESP_SOF							(0xCC) 	//Response Start of frame
#define 	RESP_EOF							(0xDD)	//Response End of frame


#define 	MAX_DATALENGTH_VAL 	(300)

#define 	STX							(0)
#define 	STATION						(1)
#define 	DATALENGTH					(2)
#define 	COMMAND						(3)
#define 	STATUS						(3)
#define 	DATA						(4)

// PC Link related communication commands

// Requests
#define		CMD_GET_DEV_ID				(0x10)
#define 	CMD_GET_COMM_STATUS			(0x17)
#define 	CMD_GET_STATUS_LOG			(0x11)

// Commads 
#define		CMD_RAISE_ISSUE 			(0x18)
#define 	CMD_RESOLVE_ISSUE			(0x19)




// Status and Error code (Application response)
#define 	RES_COMM_OK					(0x00)
#define 	RES_COMM_ERR				(0xFF)
#define		RES_TIME_OUT				(0x80)
#define 	RES_UNKNOWN_ERROR			(0x81)
#define 	RES_BAD_PARAM				(0x82)
#define 	RES_BAD_COMMAND				(0x83)

// System error codes
#define 	CHECKSUM_ERROR				(0xF0)
#define 	INVALID_TRAILER				(0xF1)
#define 	DATA_RECEIVED_OK			(0xF2)
#define 	INVALID_DATA				(0xF3)
#define 	COMMAND_RECV_TIMEOUT		(0xF4)
	
/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

// Command struct  for the data extracted .
typedef struct _PC_LINK_COMMAND_STRUCT
{
	UINT8 mDevId;
	UINT8 mCommand;  
	UINT8 mSize;
	UINT8 mData[RX_BUFFER_SIZE];
}PC_LINK_COMMAND_STRUCT;

// Command struct for the raw command packet receive from master
typedef struct _PC_LINK_COMMAND
{
	UINT8 mPacketBuf[RX_BUFFER_SIZE];
	UINT8 mReceivedindex;
	UINT8 mLeftBytes;
	UINT8 mReceiveResponse;
}PC_LINK_COMMAND;

//	UINT8 mPacketBuf[RX_BUFFER_SIZE] = {SOF, 0x00,};
//	UINT8 mReceivedindex = 0;
//	UINT8 mLeftBytes = 3;
//	UINT8 mReceiveResponse = UART_RX_STATE_IDLE;

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

extern UINT8 MyDevID;

/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

extern void InitializePCLink(void);
extern BOOL CheckPCLinkDataStatus(void);
extern rom void PCLinkTask(void);
extern void SendCmdResponce(UINT8 status, UINT8 *sndBuffer, UINT8 sndLen);

#endif
/*
*  End of pc_com_interface.h
*/



