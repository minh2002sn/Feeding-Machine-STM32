#ifndef TYPE_PASS_MENU_H
#define TYPE_PASS_MENU_H

#define MAX_PASS_LEN 50

#include "LCD_I2C.h"
#include <frame.h>
#include <menu.h>
#include <wifi_list_menu.h>

typedef struct{
	uint8_t ssid_index;
	char pass[MAX_PASS_LEN];
	uint8_t pass_digit;
}TYPE_PASS_MENU_DATA_HandleTypeDef;

void TP_MENU_Init();
void TP_MENU_Set_State(uint8_t p_ssid_index);
void TP_MENU_Set_State_NumKey(uint8_t p_key);
void TP_MENU_Set_State_SigKey(uint8_t p_key);
void TP_MENU_Set_State_Upper(uint8_t p_key);
void TP_MENU_Done_Type_Pass();
void TP_MENU_Display();

TYPE_PASS_MENU_DATA_HandleTypeDef TP_MENU_Data;

#endif
