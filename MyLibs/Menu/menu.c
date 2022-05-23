#include <menu.h>

#define SCANNING_TIMEOUT 20000

void MENU_Init(LCD_I2C_HandleTypeDef *p_hlcd){
	MENU_Data.changed = 0;
	MENU_Data.hlcd = p_hlcd;
	MENU_Data.state = MAIN_MENU;
	MAIN_MENU_Init();
	TL_MENU_Init();
	ST_MENU_Init();
	WL_MENU_Init();
	TP_MENU_Init();
}

void MENU_Handle(){
	static uint32_t t_scanning_timer = 0;
	if(!MENU_Data.changed){
		switch(MENU_Data.state){
			case MAIN_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_No_Blink(MENU_Data.hlcd);
				MAIN_MENU_Display(MENU_Data.hlcd);
				MENU_Data.changed = 1;
				break;
			case TIME_LIST_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_No_Blink(MENU_Data.hlcd);
				TL_MENU_Display();
				MENU_Data.changed = 1;
				break;
			case SET_TIME_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_Blink(MENU_Data.hlcd);
				ST_MENU_Display();
				MENU_Data.changed = 1;
				break;
			case DELETE_TIME_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_No_Blink(MENU_Data.hlcd);
				DELETE_MENU_Display();
				MENU_Data.changed = 1;
				break;
			case SMART_CONFIG_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_No_Blink(MENU_Data.hlcd);
				SM_MENU_Display();	// This function is in menu.c
				MENU_Data.changed = 1;
				break;
			case WIFI_SCANNING_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_No_Blink(MENU_Data.hlcd);
				WS_MENU_Display();	// This function is in menu.c
				MENU_Data.changed = 1;
				t_scanning_timer = HAL_GetTick();
				break;
			case WIFI_LIST_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_No_Blink(MENU_Data.hlcd);
				WL_MENU_Display();
				MENU_Data.changed = 1;
				break;
			case TYPE_PASS_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_Blink(MENU_Data.hlcd);
				TP_MENU_Display();
				MENU_Data.changed = 1;
				break;
			default:
				break;
		}
	}
	if(MENU_Data.state == WIFI_SCANNING_MENU && HAL_GetTick() - t_scanning_timer > SCANNING_TIMEOUT){
		MAIN_MENU_Set_State();
	}
}

void SM_MENU_Set_State(){
	MENU_Data.state = SMART_CONFIG_MENU;
	MENU_Data.changed = 0;
}

void SM_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	LCD_Set_Cursor(MENU_Data.hlcd, 2, 0);
	LCD_Write(MENU_Data.hlcd, "Press [#] to run");
	LCD_Set_Cursor(MENU_Data.hlcd, 4, 1);
	LCD_Write(MENU_Data.hlcd, "Smart Config");
	LCD_Set_Cursor(MENU_Data.hlcd, 0, 3);
	LCD_Write(MENU_Data.hlcd, "<[*]");
}

void WS_MENU_Set_State(){
	MENU_Data.state = WIFI_SCANNING_MENU;
	MENU_Data.changed = 0;
}

void WS_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	LCD_Set_Cursor(MENU_Data.hlcd, 4, 1);
	LCD_Write(MENU_Data.hlcd, "Scanning...");
}