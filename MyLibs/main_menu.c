#include "main_menu.h"

extern UART_HandleTypeDef huart1;

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
		{WIFI_CONNECTED, "WF:"},
		{WIFI_CONNECTING, "WF..."},
		{SMART_CONFIG_RUN, "SM..."},
};

// Frame include location (row and column index) and string displaying on LCD
static FRAME_HandleTypeDef main_menu_frame[5] = {
		{0, 0, "WF!!!"},
		{11, 0, "SUN 00:00"},
		{0, 1, "1.Set feeding time"},
		{0, 2, "2.Smart Config"},
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

void MAIN_MENU_Set_State_Time(uint8_t hour, uint8_t minute, uint8_t day){
	if(MENU_Data.state == MAIN_MENU){
		MENU_Data.changed = 0;
	}
	char time_str[8];
	uint8_t size = sizeof(DAY_Str) / sizeof(dictionary);
	for(int i = 0; i < size; i++){
		if(DAY_Str[i].index == day){
			sprintf(time_str, "%s %d:%d", DAY_Str[i].value, hour, minute);
			break;
		}
	}
	strcpy(main_menu_frame[1].str, time_str);
}

void MAIN_MENU_Set_State_WiFi(char **arg_value, uint8_t arg_num){
	char ssid_str[4] = {};
	if(arg_num == 1){
		if(strstr(arg_value[0], "SMART_CONFIG_RUN") != NULL){
			MAIN_MENU_Data.WIFI_State = SMART_CONFIG_RUN;
		}
	} else if(arg_num == 2){
		if(strstr(arg_value[1], "DISCONNECTED") != NULL){
			MAIN_MENU_Data.WIFI_State = WIFI_DISCONNECTED;
		} else if(strstr(arg_value[1], "CONNECTING") != NULL){
			MAIN_MENU_Data.WIFI_State = WIFI_CONNECTING;
		}
	} else{
		if(strstr(arg_value[1], "CONNECTED") != NULL){
			MAIN_MENU_Data.WIFI_State = WIFI_CONNECTED;
			strncpy(ssid_str, arg_value[2], (strlen(arg_value[2]) >= 3) ? 3 : strlen(arg_value[2]));
		}
	}
	if(MENU_Data.state == MAIN_MENU){
		MENU_Data.changed = 0;
	}
	uint8_t size = sizeof(WIFI_Str) / sizeof(dictionary);
	for(int i = 0; i < size; i++){
		if(WIFI_Str[i].index == MAIN_MENU_Data.WIFI_State){
			strcpy(main_menu_frame[0].str, WIFI_Str[i].value);
			if(MAIN_MENU_Data.WIFI_State == WIFI_CONNECTED){
				strcat(main_menu_frame[0].str, ssid_str);
				strcat(main_menu_frame[0].str, "...");
			}
			break;
		}
	}
}

void MAIN_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	for(int i = 0; i < 5; i++){
		LCD_Set_Cursor(MENU_Data.hlcd, main_menu_frame[i].col, main_menu_frame[i].row);
		LCD_Write(MENU_Data.hlcd, main_menu_frame[i].str);
	}
}
