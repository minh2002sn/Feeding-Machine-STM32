#include "menu.h"

void MENU_Init(LCD_I2C_HandleTypeDef *hlcd){
	MENU_Data.changed = 0;
	MENU_Data.hlcd = hlcd;
	MENU_Data.state = MAIN_MENU;
	MAIN_MENU_Init();
	TL_MENU_Init();
	ST_MENU_Init();
}

void MENU_Handle(){
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
			case CALIB_MENU:
				LCD_Backlight(MENU_Data.hlcd);
				LCD_Cursor_No_Blink(MENU_Data.hlcd);
				MENU_Data.changed = 1;
				break;
			default:
				break;
		}
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
