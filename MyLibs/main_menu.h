#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#define KALMAN_FILTER

#include "LCD_I2C.h"
#include "frame.h"
#include "menu.h"
#include "dictionary.h"

typedef enum{
	WIFI_DISCONNECTED,
	WIFI_CONNECTED,
	WIFI_CONNECTING,
	SMART_CONFIG_RUN,
}WIFI_STATE_HandleTypeDef;

typedef struct{
	WIFI_STATE_HandleTypeDef WIFI_State;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	char *WIFI_Str;
	char *TIME_Str;
}MAIN_MENU_DATA_HandleTypeDef;

void MAIN_MENU_Init();
void MAIN_MENU_Set_State();
void MAIN_MENU_Set_State_Time(uint8_t hour, uint8_t minute, uint8_t day);
void MAIN_MENU_Set_State_WiFi(char **arg_value, uint8_t arg_num);
void MAIN_MENU_Display();

MAIN_MENU_DATA_HandleTypeDef MAIN_MENU_Data;

#endif
