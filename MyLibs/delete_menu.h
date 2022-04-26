#ifndef DELETE_MENU_H
#define DELETE_MENU_H

#include "frame.h"
#include "LCD_I2C.h"
#include "menu.h"
#include "time_manage.h"

void DELETE_MENU_Init();
void DELETE_MENU_Set_State(uint8_t p_index);
void DELETE_MENU_Set_State_Delete();
void DELETE_MENU_Display();

#endif
