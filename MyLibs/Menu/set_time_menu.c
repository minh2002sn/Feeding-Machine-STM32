#include <set_time_menu.h>

//extern UART_HandleTypeDef huart2;

static FRAME_HandleTypeDef set_time_menu_frame_const[5] = {
		{0, 0, "Time: "},
		{8, 0, ":"},
		{0, 1, "Day: "},
		{0, 2, "Mass: "},
		{0, 3, "<[*]  [D]Cancel [#]>"},
};

static dictionary DAY_Str[7] = {
		{0x01, "C"},
		{0x02, "2"},
		{0x04, "3"},
		{0x08, "4"},
		{0x10, "5"},
		{0x20, "6"},
		{0x40, "7"},
};

static FRAME_HandleTypeDef set_time_menu_frame_var[4] = {
		{6, 0, ""},
		{9, 0, ""},
		{5, 1, ""},
		{6, 2, ""},
};

static uint8_t number_of_digit[4] = {2, 2, 7, 5};
static uint16_t data_max[4] = {23, 59, 0b01111111, 1000};

static void create_day_string(char p_str[], uint8_t p_day){
	for(int i = 0; i < 7; i++){
		if(p_day & (1 << i)){
			for(int j = 0; j < 7; j++){
				if(DAY_Str[j].index == (1 << i)){
					strcat(p_str, DAY_Str[j].value);
					break;
				}
			}
		}
	}
}

void ST_MENU_Init(){

}

void ST_MENU_Set_State(){
	MENU_Data.state = SET_TIME_MENU;
	MENU_Data.changed = 0;
	ST_MENU_Data.digit = 0;
	for(int i = 0; i < 4; i++) ST_MENU_Data.time_data[i] = 0;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 21; j++){
			set_time_menu_frame_var[i].str[j] = 0;
		}
	}
	ST_MENU_Data.setting_type = HOUR_TYPE;
}

void ST_MENU_Set_State_NumKey(uint8_t p_key){
	MENU_Data.state = SET_TIME_MENU;
	MENU_Data.changed = 0;
	if(ST_MENU_Data.setting_type == DAY_TYPE){
		if(p_key == 'C'){
			uint8_t t_data = 1;
			if(ST_MENU_Data.time_data[DAY_TYPE] & t_data){
				ST_MENU_Data.time_data[DAY_TYPE] &= ~t_data;
			} else{
				ST_MENU_Data.time_data[DAY_TYPE] |= t_data;
			}
		} else if(p_key >= '1' && p_key <= '7'){
			uint8_t t_data = 1 << (p_key - '1');
			if(ST_MENU_Data.time_data[DAY_TYPE] & t_data){
				ST_MENU_Data.time_data[DAY_TYPE] &= ~t_data;
			} else{
				ST_MENU_Data.time_data[DAY_TYPE] |= t_data;
			}
		}
		char t_str[8] = {};
		create_day_string(t_str, ST_MENU_Data.time_data[DAY_TYPE]);
		ST_MENU_Data.digit = (!strlen(t_str)) ? strlen(t_str) : (strlen(t_str) - 1);
		strcpy(set_time_menu_frame_var[DAY_TYPE].str, t_str);
	} else{
		if(ST_MENU_Data.digit < number_of_digit[ST_MENU_Data.setting_type]){
			set_time_menu_frame_var[ST_MENU_Data.setting_type].str[ST_MENU_Data.digit] = (char)p_key;
			if(ST_MENU_Data.digit + 1 < number_of_digit[ST_MENU_Data.setting_type]){
				ST_MENU_Data.digit++;
			}
		}
		if(ST_MENU_Data.digit + 1 == number_of_digit[ST_MENU_Data.setting_type]){
			if(atoi(set_time_menu_frame_var[ST_MENU_Data.setting_type].str) > data_max[ST_MENU_Data.setting_type]){
				char t_str[10] = {};
				sprintf(t_str, "%d", data_max[ST_MENU_Data.setting_type]);
				strcpy(set_time_menu_frame_var[ST_MENU_Data.setting_type].str, t_str);
			}
		}
	}
}

void ST_MENU_Set_State_SigKey(uint8_t p_key){
	MENU_Data.state = SET_TIME_MENU;
	MENU_Data.changed = 0;
	if(p_key == '#'){
		if(ST_MENU_Data.setting_type != CHECK_AGAIN){
			ST_MENU_Data.setting_type++;
			ST_MENU_Data.digit = 0;
		} else if(ST_MENU_Data.setting_type == CHECK_AGAIN){
			ST_MENU_Data.time_data[0] = atoi(set_time_menu_frame_var[0].str);
			ST_MENU_Data.time_data[1] = atoi(set_time_menu_frame_var[1].str);
			ST_MENU_Data.time_data[3] = atoi(set_time_menu_frame_var[3].str);
			TIME_Add(ST_MENU_Data.time_data[2], ST_MENU_Data.time_data[0], ST_MENU_Data.time_data[1], ST_MENU_Data.time_data[3]);
			TL_MENU_Set_State(0);
		}
	} else if(p_key == '*'){
		if(ST_MENU_Data.digit > 0){
			set_time_menu_frame_var[ST_MENU_Data.setting_type].str[ST_MENU_Data.digit] = 0;
			ST_MENU_Data.digit--;
		} else{
			if(ST_MENU_Data.setting_type > 0){
				ST_MENU_Data.setting_type--;
				ST_MENU_Data.digit = strlen(set_time_menu_frame_var[ST_MENU_Data.setting_type].str) - 1;
			}
		}

	}
}

void ST_MENU_Display(){
	if(ST_MENU_Data.setting_type == CHECK_AGAIN){
		LCD_Cursor_No_Blink(MENU_Data.hlcd);
	} else{
		LCD_Clear(MENU_Data.hlcd);
		LCD_Cursor_Blink(MENU_Data.hlcd);
		for(int i = 0; i < 5; i++){
			LCD_Set_Cursor(MENU_Data.hlcd, set_time_menu_frame_const[i].col, set_time_menu_frame_const[i].row);
			LCD_Write(MENU_Data.hlcd, set_time_menu_frame_const[i].str);
		}
		for(int i = 0; i < 4; i++){
			LCD_Set_Cursor(MENU_Data.hlcd, set_time_menu_frame_var[i].col, set_time_menu_frame_var[i].row);
			LCD_Write(MENU_Data.hlcd, set_time_menu_frame_var[i].str);
		}
		LCD_Set_Cursor(MENU_Data.hlcd, set_time_menu_frame_var[ST_MENU_Data.setting_type].col + ST_MENU_Data.digit, set_time_menu_frame_var[ST_MENU_Data.setting_type].row);
	}
}
