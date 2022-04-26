#ifndef TIME_LIST_MENU_H
#define TIME_LIST_MENU_H

#include "LCD_I2C.h"
#include "frame.h"
#include "menu.h"
#include "time_manage.h"
#include "dictionary.h"

#define NUMBER_OF_CHOICE (MAX_OPTIONS + 1)

#define ROW_3 "<[*]            [#]>"

typedef struct{
	uint8_t first_line;
	char list_str[NUMBER_OF_CHOICE][21];
}TL_MENU_DATA_HandleTypeDef;

void TL_MENU_Init();
void TL_MENU_Set_State(uint8_t p_first_line);
void TL_MENU_Display();

TL_MENU_DATA_HandleTypeDef TL_MENU_Data;

#endif
