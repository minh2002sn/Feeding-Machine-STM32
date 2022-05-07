#ifndef WIFI_LIST_H
#define WIFI_LIST_H

#include "LCD_I2C.h"
#include "frame.h"
#include "menu.h"

typedef struct{
	uint8_t first_line;
	uint8_t num_of_wifi;
	char wifi_list[6][21];
	uint8_t enable_receive_ssid;
}WIFI_LIST_MENU_DATA_HandleTypeDef;

void WL_MENU_Init();
void WL_MENU_Enable_Recieve_SSID();
void WL_MENU_Disable_Recieve_SSID();
uint8_t WL_MENU_Is_Enable_Receiving_SSID();
void WL_MENU_Set_State(uint8_t p_first_line);
void WL_MENU_Display();

WIFI_LIST_MENU_DATA_HandleTypeDef WL_MENU_Data;

#endif
