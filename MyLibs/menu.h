#ifndef MENU_H
#define MENU_H

#include "LCD_I2C.h"
#include "main_menu.h"
#include "time_list_menu.h"
#include "set_time_menu.h"
#include "delete_menu.h"
#include "dictionary.h"

typedef enum{
	MAIN_MENU,
	TIME_LIST_MENU,
	SET_TIME_MENU,
	DELETE_TIME_MENU,
	SMART_CONFIG_MENU,
	CALIB_MENU,
}MENU_STATE_HandleTypeDef;

typedef struct{
	uint8_t changed;
	LCD_I2C_HandleTypeDef *hlcd;
	MENU_STATE_HandleTypeDef state;
}MENU_DATA_HandleTypeDef;

void MENU_Init(LCD_I2C_HandleTypeDef *p_hlcd);
void MENU_Handle();
void SM_MENU_Set_State();
void SM_MENU_Display();

MENU_DATA_HandleTypeDef MENU_Data;

#endif
