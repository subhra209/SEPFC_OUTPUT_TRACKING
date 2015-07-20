#include "Externalinterrupt.h"

 UINT8 INPUT_FLAG[2]={0,0};

void External_Init(void)
{
	INTCON = 0x80;

	OpenRB0INT( FALLING_EDGE_INT & PORTB_PULLUPS_OFF & PORTB_INT_PRIO_HIGH );
	OpenRB1INT( FALLING_EDGE_INT & PORTB_PULLUPS_OFF & PORTB_INT_PRIO_HIGH );
	
	INTCONbits.INT0IE = 1;
	INTCON3bits.INT1IE = 1;

	INTCONbits.INT0IF =0;
	INTCON3bits.INT1IF =0;
}


#pragma interrupt RB0INT_ISR
void RB0INT_ISR(void)
{
 	INTCONbits.INT0IF =0;

	DISABLE_TMR0_INTERRUPT();
	if(debounceCountB0 < 100)
		return;
	else
	{
  		INPUT_FLAG[0] = 1;
		debounceCountB0 = 0;
	}
	ENABLE_TMR0_INTERRUPT();

 
}
	


#pragma interrupt RB1INT_ISR
void RB1INT_ISR(void)
{

    INTCON3bits.INT1IF =0;

	DISABLE_TMR0_INTERRUPT();
	if(debounceCountB1 < 100)
		return;
	else
	{
  		INPUT_FLAG[1] = 1;
		debounceCountB1 = 0;
	}
	ENABLE_TMR0_INTERRUPT();



}	