





	
	
enum
{
	UI_MSG_LINE = 0,
	UI_MSG_MENU,
	UI_MSG_REFERENCE,
	UI_MSG_OPERATORS,
	UI_MSG_HDY,
	UI_MSG_SDY,
	UI_MSG_BREAK,
	UI_MSG_NAME,
	UI_MSG_OPS
	
};
		

enum
{
	ISSUE_0 = 0,
	ISSUE_1 ,
	ISSUE_2,
	ISSUE_3
};



void UI_init(void);
void UI_task(void);
void UI_reset(void);
