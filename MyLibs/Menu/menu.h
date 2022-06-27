#ifndef MENU_H
#define MENU_H

#include "delete_menu.h"
#include "dictionary.h"
#include "main_menu.h"
#include "set_time_menu.h"
#include "time_list_menu.h"
#include "type_pass_menu.h"
#include "wifi_list_menu.h"
#include "LCD_I2C.h"

typedef enum{
	MAIN_MENU,
	TIME_LIST_MENU,
	SET_TIME_MENU,
	DELETE_TIME_MENU,
	SMART_CONFIG_MENU,
	WIFI_SCANNING_MENU,
	WIFI_LIST_MENU,
	TYPE_PASS_MENU,
	CALIB_MENU,
}MENU_STATE_HandleTypeDef;

typedef struct{
	uint8_t changed;
	LCD_I2C_HandleTypeDef *hlcd;
	MENU_STATE_HandleTypeDef state;
}MENU_DATA_HandleTypeDef;

void MENU_Init(LCD_I2C_HandleTypeDef *p_hlcd);
void MENU_Handle();
//void SM_MENU_Set_State();
//void SM_MENU_Display();
void WS_MENU_Set_State();
void WS_MENU_Display();

extern MENU_DATA_HandleTypeDef MENU_Data;

#endif
