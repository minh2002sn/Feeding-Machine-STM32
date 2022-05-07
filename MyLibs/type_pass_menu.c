#include "type_pass_menu.h"

extern UART_HandleTypeDef huart2;

static char key_map[4][4][9] = {
		{"1.,?!",	"2abc",		"3def",		"A`@#$%^"},	// Error with '~' and '%' characters
		{"4ghi", 	"5jkl",		"6mno",		"B&*()-_+"},
		{"7pqrs",	"8tuv",		"9wsyz",	"C=[]{}|"},		// Error with '\' character
		{"",		"0",		"",			"D:;\"'<>/"},
};
static char last_key = '1';
static int8_t key_index = -1;

static FRAME_HandleTypeDef type_pass_menu_frame[3] = {
		{0, 0, "SSID: "},
		{0, 1, "PASS: "},
		{0, 3, "<[*]            [#]>"}
};

static FRAME_HandleTypeDef ssid_frame = {6, 0, ""};
static FRAME_HandleTypeDef pass_frame = {6, 1, ""};

void TP_MENU_Init(){
	TP_MENU_Data.ssid_index = 0;
	for(int i = 0; i < 50; i++){
		TP_MENU_Data.pass[i] = 0;
	}
}

void TP_MENU_Set_State(uint8_t p_ssid_index){
	MENU_Data.state = TYPE_PASS_MENU;
	MENU_Data.changed = 0;
	TP_MENU_Data.ssid_index = p_ssid_index;
	for(int i = 0; i < 50; i++){
		TP_MENU_Data.pass[i] = 0;
	}
	TP_MENU_Data.pass_digit = 0;
	last_key = '1';
	char t_ssid_str[21] = {};
	strcpy(t_ssid_str, WL_MENU_Data.wifi_list[TP_MENU_Data.ssid_index]);
	strncpy(ssid_frame.str, t_ssid_str, ((strlen(t_ssid_str) > 14) ? 14 : strlen(t_ssid_str)));
}

void TP_MENU_Set_State_NumKey(uint8_t p_key){
	MENU_Data.state = TYPE_PASS_MENU;
	MENU_Data.changed = 0;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(p_key == key_map[i][j][0]){
				if(key_map[i][j][key_index + 1] == 0 || p_key != last_key){
					key_index = 0;
				} else{
					key_index++;
				}
				TP_MENU_Data.pass[TP_MENU_Data.pass_digit] = key_map[i][j][key_index];
				break;
			}
		}
	}
	strcpy(pass_frame.str, TP_MENU_Data.pass);
	last_key = p_key;
}

void TP_MENU_Set_State_SigKey(uint8_t p_key){
	MENU_Data.state = TYPE_PASS_MENU;
	MENU_Data.changed = 0;
	if(p_key == '*'){
		if(TP_MENU_Data.pass_digit > 0){
			TP_MENU_Data.pass[TP_MENU_Data.pass_digit] = 0;
			TP_MENU_Data.pass_digit--;
		}
	} else if(p_key == '#'){
		if(TP_MENU_Data.pass_digit < MAX_PASS_LEN && TP_MENU_Data.pass[TP_MENU_Data.pass_digit] != 0){
			TP_MENU_Data.pass_digit++;
			key_index = 7;
		}
	}
	strcpy(pass_frame.str, TP_MENU_Data.pass);
}

void TP_MENU_Set_State_Upper(uint8_t p_key){
	MENU_Data.state = TYPE_PASS_MENU;
	MENU_Data.changed = 0;
	char t_char = TP_MENU_Data.pass[TP_MENU_Data.pass_digit];
	if(p_key == last_key){
		if(t_char > 'a' && t_char < 'z'){
			TP_MENU_Data.pass[TP_MENU_Data.pass_digit] -= 32;
		} else if(t_char > 'A' && t_char < 'Z'){
			TP_MENU_Data.pass[TP_MENU_Data.pass_digit] += 32;
		}
	}
	strcpy(pass_frame.str, TP_MENU_Data.pass);
}

void TP_MENU_Done_Type_Pass(){
	TP_MENU_Data.pass[++TP_MENU_Data.pass_digit] = 0;
	uint8_t t_tx_buffer[70] = {};
	sprintf((char *)t_tx_buffer, "CONNECT_WIFI %d %s\n", TP_MENU_Data.ssid_index, TP_MENU_Data.pass);
	HAL_UART_Transmit(&huart2, t_tx_buffer, strlen((char *)t_tx_buffer), 500);
}

void TP_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	for(int i = 0; i < 3; i++){
		LCD_Set_Cursor(MENU_Data.hlcd, type_pass_menu_frame[i].col, type_pass_menu_frame[i].row);
		LCD_Write(MENU_Data.hlcd, type_pass_menu_frame[i].str);
	}
	LCD_Set_Cursor(MENU_Data.hlcd, ssid_frame.col, ssid_frame.row);
	LCD_Write(MENU_Data.hlcd, ssid_frame.str);
	LCD_Set_Cursor(MENU_Data.hlcd, pass_frame.col, pass_frame.row);
	LCD_Write(MENU_Data.hlcd, pass_frame.str);
	LCD_Set_Cursor(MENU_Data.hlcd, pass_frame.col + TP_MENU_Data.pass_digit, pass_frame.row);
}
