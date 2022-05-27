#include <delete_menu.h>

static uint8_t current_index = 0;
extern FRAME_HandleTypeDef time_list_menu_frame[4];

void DELETE_MENU_Init(){

}

void DELETE_MENU_Set_State(uint8_t p_index){
	MENU_Data.state = DELETE_TIME_MENU;
	MENU_Data.changed = 0;
	current_index = p_index;
}

void DELETE_MENU_Set_State_Delete(){
	TIME_Delete(current_index);
	TL_MENU_Set_State(0);
}

void DELETE_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	LCD_Cursor_No_Blink(MENU_Data.hlcd);
	for(int i = 0; i < 3; i++){
		LCD_Set_Cursor(MENU_Data.hlcd, time_list_menu_frame[i].col, time_list_menu_frame[i].row);
		LCD_Write(MENU_Data.hlcd, time_list_menu_frame[i].str);
//		if(strlen(TL_MENU_Data.list_str[TL_MENU_Data.first_line+i]) != 0){
//			char temp_c[2] = {i + '1', '\0'};
//			LCD_Set_Cursor(MENU_Data.hlcd, 0, i);
//			LCD_Write(MENU_Data.hlcd, TL_MENU_Data.list_str[TL_MENU_Data.first_line+i]);
//			LCD_Set_Cursor(MENU_Data.hlcd, 0, i);
//			LCD_Write(MENU_Data.hlcd, temp_c);
//		}
	}
	LCD_Set_Cursor(MENU_Data.hlcd, 0, 3);
	LCD_Write(MENU_Data.hlcd, "<[*]       [D]Delete");
}
