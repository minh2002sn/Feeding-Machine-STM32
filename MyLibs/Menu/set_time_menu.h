#ifndef SET_TIME_MENU_H
#define SET_TIME_MENU_H

#include "dictionary.h"
#include "frame.h"
#include "menu.h"
#include "time_list_menu.h"
#include "time_manage.h"
#include "LCD_I2C.h"

typedef enum{
	HOUR_TYPE,
	MINUTE_TYPE,
	DAY_TYPE,
	MASS_TYPE,
	CHECK_AGAIN,
}SETTING_TYPE_HandleTypeDef;

typedef struct{
	uint8_t digit;
	uint16_t time_data[4];
	SETTING_TYPE_HandleTypeDef setting_type;
}ST_MENU_Data_HandleTypeDef;

void ST_MENU_Init();
void ST_MENU_Set_State();
void ST_MENU_Set_State_NumKey(uint8_t p_key);
void ST_MENU_Set_State_SigKey(uint8_t p_key);
void ST_MENU_Display();

extern ST_MENU_Data_HandleTypeDef ST_MENU_Data;

#endif
