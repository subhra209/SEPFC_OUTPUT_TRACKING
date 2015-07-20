
#include "board.h"
#include "portb.h"

void portb_init(void);
void portb_task(void);	
void RB0INT_ISR(void);
void RB1INT_ISR(void);
extern UINT16 debounceCountB0;
extern UINT16 debounceCountB1;
