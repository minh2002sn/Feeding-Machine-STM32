#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#define KALMAN_FILTER

#include "LCD_I2C.h"
#include "frame.h"
#include "menu.h"
#include "dictionary.h"

typedef enum{
	WIFI_CONNECTED,
	WIFI_DISCONNECTED,
	WIFI_CONNECTING,
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
void MAIN_MENU_Set_State_Time(uint8_t p_hour, uint8_t p_minute, uint8_t p_day);
void MAIN_MENU_Set_State_WiFi(WIFI_STATE_HandleTypeDef p_status);
void MAIN_MENU_Display();

extern MAIN_MENU_DATA_HandleTypeDef MAIN_MENU_Data;

#endif
