/*
*------------------------------------------------------------------------------
* keypad_driver.c
*
* keypad driver module.
*
* (C)2008 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/
/*
* Simple Keypad library, for a 4 - column x 4 - row Keypad.
*
* Key arrangement is:     ------------
*                          1  2  3	c  
*                          4  5  6	u
*                          7  8  9	d
*                          r  0  ok	m
*                         ------------
*/
/*
*------------------------------------------------------------------------------
* File				: keypad_driver.c
------------------------------------------------------
*
* Revision 0.1 19/07/20009 Sam
* Revised key codes to suit the current hardware.
* Revision 0.0 03/10/2008 Sam
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
#include "keypad_driver.h"
#include "lcd_driver.h"



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

#define KEYPAD_RECV_BUFF_LEN 	(6)

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

/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

static UINT8 keypadReceiveBuffer[KEYPAD_RECV_BUFF_LEN + 1][2];
static UINT8 kepadInReadIndex;     	// Data in buffer that has been read
static UINT8 keypadInWaitingIndex;  // Data in buffer not yet read
static UINT8 lastValidKey = KEYPAD_NO_NEW_DATA;
static UINT8 duration; 


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

static BOOL scanKeypad(UINT8* const pkeyNormal);
/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/
/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/
/*
*------------------------------------------------------------------------------
* void InitializeKeypad(void)
*
* Summary	: Initialize keypad buffer indexes
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/
void InitializeKeypad(void)
{
	kepadInReadIndex = 0;
   	keypadInWaitingIndex = 0;
	//Set the task up to run	
	
}

/*
*------------------------------------------------------------------------------
* void UpdateKeypadTask(void)
*
* Summary	: The function update the key pressed into keypad buffer
*			  This must be schedule at approx every 50 - 200 ms .
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/
rom void UpdateKeypadTask(void)
{
	UINT8 currentKey, functionKey;

	duration++;
   	// Scan Keypad here...
   	if (scanKeypad(&currentKey) == 0)
    {
    	// No new Key data - just return
      	return;
    }

   	// Want to read into index 0, if old data has been read
   	// (simple ~circular buffer)
   	if (keypadInWaitingIndex == kepadInReadIndex)
    {
    	keypadInWaitingIndex = 0;
      	kepadInReadIndex = 0;
    }

   	// Load Keypad data into buffer
   	keypadReceiveBuffer[keypadInWaitingIndex][0] = currentKey;
	keypadReceiveBuffer[keypadInWaitingIndex][1] = duration;
   	
   	if(keypadInWaitingIndex < KEYPAD_RECV_BUFF_LEN)
    {
    	// Increment without overflowing buffer
      	keypadInWaitingIndex++;
    }
	duration = 0;
}


#ifdef INPUT_SIMULATION

#define NO_INPUTS  36

static rom UINT16 Key[][2] = {{0x4,40},{0x10,30},{0x8,50},{0x10,50},{0x01,50},
								{0x10,40},{0x1,30},{0x2,50},{0x9,50},{0xa,50},{0x0F,50},
								{0x4,40},{0x10,30},{0x2,50},{0x10,50},{0x6,50},{0xF,50},
								{0x4,40},{0x10,30},{0xD,50},{0xF,50},{0x6,50},{0x7,50},
								{0x8,40},{0x9,30},{0xa,50},{0xb,50},{0xc,50},{0xd,50},
								{0xe,40},{0xf,30},{0x1,50},{0x2,50},{0x3,50},{0x4,50},
								{0x5,50},{0x6,50},{0x7,50},
								{0x8,40},{0x9,30},{0xa,50},{0xb,50},{0xc,50},{0xd,50},{0xe,50},
								{0xf,40},{0x10,30},{0x1,50},{0x2,50},{0x3,50},{0x10,50},
								
							{0x01,50},{0x0c,50},{0x09,50},{0x0c,50},
							
							{0x01,50},{0x02,50},{0x0c,50},{0x01,50},{0x0c,50},{0x03,50},{0x01,50},{0xc,50},
							{0xb,50},{0x02,50},{0xc,50},{0x02,50},{0xc,50},{0x01,50},{0xc,50},
								{0x03,50},{0x01,50},{0x02,50},{0x03,50},{0x01,50},{0x02,50},{0x03,50},
							  {0x01,50},{0x02,50},{0x03,50},{0x01,50},{0x02,50},{0x03,50},{0x01,50},{0x02,50},{0x18,45},
								};
UINT8 keyIndex = 0;
UINT8 delayCount = 5;


/*
*------------------------------------------------------------------------------
* BOOL GetDataFromKeypadBuffer(UINT8* const pkeyNormal)
*
* Summary	: This extracts data from the keypad buffer.
*
* Input		: UINT8* const pkeyNormal  - pointer to get normal key entry
*
* Output	: BOOL - 1 - if data exisist in the buffer
*			         0 - if no data in the buffer
*------------------------------------------------------------------------------
*/
BOOL GetDataFromKeypadBuffer(UINT8* const pkey, UINT8* pduration)
{

	if( keyIndex >= NO_INPUTS)
	{
		return 0;
		keyIndex = 0;
	}

	if( Key[keyIndex][0] != 0xFF && Key[keyIndex][0] != 0x18)
	{
		*pkey = Key[keyIndex][0];
		*pduration = Key[keyIndex][1];
		keyIndex++;
		return 1;
	}
	else if( Key[keyIndex][0] == 0x18)
	{
		DelayMs(10);
		*pkey = Key[keyIndex][0];
		*pduration = Key[keyIndex][1];
		keyIndex++;
		return 1;
	}	
	DelayMs(10);
	keyIndex++;
	return 0;
}
#else
/*
*------------------------------------------------------------------------------
* BOOL GetDataFromKeypadBuffer(UINT8* const pkeyNormal)
*
* Summary	: This extracts data from the keypad buffer.
*
* Input		: UINT8* const pkeyNormal  - pointer to get normal key entry
*
* Output	: BOOL - 1 - if data exisist in the buffer
*			         0 - if no data in the buffer
*------------------------------------------------------------------------------
*/
BOOL GetDataFromKeypadBuffer(UINT8* const pkey, UINT8* pduration)
{
	// If there is new data in the buffer
   	if (kepadInReadIndex < keypadInWaitingIndex)
    {
  		*pkey = keypadReceiveBuffer[kepadInReadIndex][0];
		*pduration = keypadReceiveBuffer[kepadInReadIndex][1];
      	kepadInReadIndex++;
	
    	return 1;
    }
   	return 0;
}

#endif
/*
*------------------------------------------------------------------------------
* void ClearKeytpadBuffer(void)
*
* Summary	: Clears the keypad buffer by resetting read and write index
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void ClearKeytpadBuffer(void)
{
	keypadInWaitingIndex = 0;
   	kepadInReadIndex = 0;
}

/*
*------------------------------------------------------------------------------
* BOOL scanKeypad(UINT8* const pkeyNormal)
*
* Summary	: This function is called from scheduled UpdateKeypadTask function.
*
* Input		: UINT8* const pkeyNormal -  pointer to get normal key data
*
* Output	: BOOL - 0 - if no new key data
*       	         1 - if there is new key data	
*
* Note		: Must be edited as required to match Key labels.
*------------------------------------------------------------------------------
*/
BOOL scanKeypad(UINT8* const pkeyNormal)
{
	static UINT8 oldKey;

   	UINT8 currentKey = KEYPAD_NO_NEW_DATA;

	KBD_ROW0_DIR = PORT_IN;
	KBD_ROW1_DIR = PORT_IN;
	KBD_ROW2_DIR = PORT_IN;
	KBD_ROW3_DIR = PORT_IN;

	KBD_COL0_DIR = PORT_OUT;
	KBD_COL1_DIR = PORT_OUT;
	KBD_COL2_DIR = PORT_OUT;
	KBD_COL3_DIR = PORT_OUT;

   	
KBD_COL0 = 0; // Scanning column 0
    	if (KBD_ROW0 == 0) currentKey = 0x01;
     	if (KBD_ROW1 == 0) currentKey = 0x05;
      	if (KBD_ROW2 == 0) currentKey = 0x09;
      	if (KBD_ROW3 == 0) currentKey = 0x0D;
   	KBD_COL0 = 1;

   	KBD_COL1 = 0; // Scanning column 1
   		if (KBD_ROW0 == 0) currentKey = 0x02;
    	if (KBD_ROW1 == 0) currentKey = 0x06;
      	if (KBD_ROW2 == 0) currentKey = 0x0A;
      	if (KBD_ROW3 == 0) currentKey = 0x0E;
   	KBD_COL1 = 1;

   	KBD_COL2 = 0; // Scanning column 2
    	if (KBD_ROW0 == 0) currentKey = 0x03;
      	if (KBD_ROW1 == 0) currentKey = 0x07;
      	if (KBD_ROW2 == 0) currentKey = 0x0B;
      	if (KBD_ROW3 == 0) currentKey = 0x0F;
   	KBD_COL2 = 1;

	KBD_COL3 = 0; // Scanning column 3
    	if (KBD_ROW0 == 0) currentKey = 0x04;
      	if (KBD_ROW1 == 0) currentKey = 0x08;
      	if (KBD_ROW2 == 0) currentKey = 0x0C;
      	if (KBD_ROW3 == 0) currentKey = 0x10;
   	KBD_COL3 = 1;
/*
	KBD_COL4 = 0; // Scanning column 4
    	if (KBD_ROW0 == 0) currentKey = 0x17;
      	if (KBD_ROW1 == 0) currentKey = 0x14;
      	if (KBD_ROW2 == 0) currentKey = 0x11;
      	if (KBD_ROW3 == 0) currentKey = 0x0E;
   	KBD_COL4 = 1;

	KBD_COL5 = 0; // Scanning column 5
    	if (KBD_ROW0 == 0) currentKey = 0x18;
      	if (KBD_ROW1 == 0) currentKey = 0x15;
      	if (KBD_ROW2 == 0) currentKey = 0x12;
      	if (KBD_ROW3 == 0) currentKey = 0x0F;
   	KBD_COL5 = 1;
*/

   	if (currentKey == KEYPAD_NO_NEW_DATA)
    {
    	// No key pressed (or just a function key)
      	oldKey = KEYPAD_NO_NEW_DATA;
      	lastValidKey = KEYPAD_NO_NEW_DATA;
      	return 0;  // No new data
    }

	// A Key has been pressed: debounce by checking twice
   	if (currentKey == oldKey)
    {
    	// A valid (debounced) key press has been detected

      	// Must be a new key to be valid - no 'auto repeat'
      	if (currentKey != lastValidKey)
        {
        	// New key!
         	*pkeyNormal = currentKey;
//			putCharToLcd(currentKey + '0');
         	lastValidKey = currentKey;
  			//PutcUARTRam(currentKey);     // Send STATUS
         	return 1;
     	}
   	}

   	// No new data
   	oldKey = currentKey;
   	return 0;
}


/*
*  End of keypad_driver.c
*/