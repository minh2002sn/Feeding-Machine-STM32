#include "main_menu.h"

extern UART_HandleTypeDef huart1;
MAIN_MENU_DATA_HandleTypeDef MAIN_MENU_Data;

// Day of week strings display on LCD
static dictionary DAY_Str[7] = {
		{0x01, "SUN"},
		{0x02, "MON"},
		{0x04, "TUE"},
		{0x08, "WED"},
		{0x10, "THU"},
		{0x20, "FRI"},
		{0x40, "SAT"},
};

// WiFi state strings display on LCD
dictionary WIFI_Str[4] = {
		{WIFI_DISCONNECTED, "WF!!!"},
		{WIFI_CONNECTED, "WF"},
		{WIFI_CONNECTING, "WF..."},
};

// Frame include location (row and column index) and string displaying on LCD
static FRAME_HandleTypeDef main_menu_frame[5] = {
		{0, 0, "WF!!!"},
		{11, 0, "SUN 00:00"},
		{0, 1, "1.Set feeding time"},
		{0, 2, "2.Connect wifi"},
		{2, 3, "Press [1] or [2]"},
};

void MAIN_MENU_Init(){
	MAIN_MENU_Data.WIFI_State = WIFI_DISCONNECTED;
	MAIN_MENU_Data.day = 1;
	MAIN_MENU_Data.hour = 0;
	MAIN_MENU_Data.minute = 0;
	MAIN_MENU_Data.WIFI_Str = WIFI_Str[0].value;
	MAIN_MENU_Data.TIME_Str = "SUN 0:0";
}

void MAIN_MENU_Set_State(){
	MENU_Data.state = MAIN_MENU;
	MENU_Data.changed = 0;
}

void MAIN_MENU_Set_State_Time(uint8_t p_hour, uint8_t p_minute, uint8_t p_day){
	if(MENU_Data.state == MAIN_MENU){
		MENU_Data.changed = 0;
	}
	char t_time_str[8];
	uint8_t t_size = sizeof(DAY_Str) / sizeof(dictionary);
	for(int i = 0; i < t_size; i++){
		if(DAY_Str[i].index == p_day){
			sprintf(t_time_str, "%s %02d:%02d", DAY_Str[i].value, p_hour, p_minute);
			break;
		}
	}
	strcpy(main_menu_frame[1].str, t_time_str);
}

void MAIN_MENU_Set_State_WiFi(WIFI_STATE_HandleTypeDef p_status){
	MAIN_MENU_Data.WIFI_State = p_status;
	if(MENU_Data.state == MAIN_MENU){
		MENU_Data.changed = 0;
	}
	uint8_t t_size = sizeof(WIFI_Str) / sizeof(dictionary);
	for(int i = 0; i < t_size; i++){
		if(WIFI_Str[i].index == MAIN_MENU_Data.WIFI_State){
			strcpy(main_menu_frame[0].str, WIFI_Str[i].value);
			break;
		}
	}
}

void MAIN_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	LCD_Cursor_No_Blink(MENU_Data.hlcd);
	for(int i = 0; i < 5; i++){
		LCD_Set_Cursor(MENU_Data.hlcd, main_menu_frame[i].col, main_menu_frame[i].row);
		LCD_Write(MENU_Data.hlcd, main_menu_frame[i].str);
	}
}
