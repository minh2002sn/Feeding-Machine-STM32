#include <wifi_list_menu.h>

extern UART_HandleTypeDef huart1;

FRAME_HandleTypeDef wifi_list_menu_frame[4] = {
		{0, 0, ""},
		{0, 1, ""},
		{0, 2, ""},
		{0, 3, "<[*]            [#]>"}
};

void WL_MENU_Init(){
	WL_MENU_Data.first_line = 0;
	WL_MENU_Data.num_of_wifi = 0;
	WL_MENU_Data.enable_receive_ssid = 0;
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 21; j++){
			WL_MENU_Data.wifi_list[i][j] = 0;
		}
	}
}

void WL_MENU_Enable_Recieve_SSID(){
	WL_MENU_Data.first_line = 0;
	WL_MENU_Data.num_of_wifi = 0;
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 21; j++){
			WL_MENU_Data.wifi_list[i][j] = 0;
		}
	}
	WL_MENU_Data.enable_receive_ssid = 1;
}

void WL_MENU_Disable_Recieve_SSID(){
	WL_MENU_Data.enable_receive_ssid = 0;
}

uint8_t WL_MENU_Is_Enable_Receiving_SSID(){
	return WL_MENU_Data.enable_receive_ssid;
}

void WL_MENU_Set_State(uint8_t p_first_line){
	if(p_first_line > 3) return;
	MENU_Data.state = WIFI_LIST_MENU;
	MENU_Data.changed = 0;
	WL_MENU_Data.first_line = p_first_line;
	for(int i = 0; i < 3; i++){
		if((i + WL_MENU_Data.first_line) < WL_MENU_Data.num_of_wifi){
			wifi_list_menu_frame[i].str[0] = i + '1';
			wifi_list_menu_frame[i].str[1] = '.';
			wifi_list_menu_frame[i].str[2] = '\0';
			strcat(wifi_list_menu_frame[i].str, WL_MENU_Data.wifi_list[WL_MENU_Data.first_line + i]);
		} else{
			strcpy(wifi_list_menu_frame[i].str, "");
		}
	}
//	for(int i = 0; i < WL_MENU_Data.num_of_wifi; i++){
//		HAL_UART_Transmit(&huart1, WL_MENU_Data.wifi_list[i], strlen(WL_MENU_Data.wifi_list[i]), 500);
//		HAL_UART_Transmit(&huart1, "\n", 1, 500);
//	}
}

void WL_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	LCD_Cursor_No_Blink(MENU_Data.hlcd);
	for(int i = 0; i < 4; i++){
		LCD_Set_Cursor(MENU_Data.hlcd, wifi_list_menu_frame[i].col, wifi_list_menu_frame[i].row);
		LCD_Write(MENU_Data.hlcd, wifi_list_menu_frame[i].str);
	}
}
