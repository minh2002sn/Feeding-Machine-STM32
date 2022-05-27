#include <time_list_menu.h>

static dictionary DAY_Str[7] = {
		{0x01, "C"},
		{0x02, "2"},
		{0x04, "3"},
		{0x08, "4"},
		{0x10, "5"},
		{0x20, "6"},
		{0x40, "7"},
};

FRAME_HandleTypeDef time_list_menu_frame[4] = {
		{0, 0, ""},
		{0, 1, ""},
		{0, 2, ""},
		{0, 3, "<[*]            [#]>"}
};

static void create_day_string(char str[], uint8_t day){
	for(int i = 0; i < 7; i++){
		if(day & (1 << i)){
			for(int j = 0; j < 7; j++){
				if(DAY_Str[j].index == (1 << i)){
					strcat(str, DAY_Str[j].value);
					break;
				}
			}
		}
	}
}

static void update_time_list(){
	for(int i = 0; i < TIME_Data.len; i++){
		char day_str[8] = {};
		create_day_string(day_str, TIME_Data.flash_data[i].day);
		char time_str[21] = {};
		sprintf(time_str, " .%d:%d %s %d", TIME_Data.flash_data[i].hour, TIME_Data.flash_data[i].minute, day_str, TIME_Data.flash_data[i].mass);
		strcpy(TL_MENU_Data.list_str[i] , time_str);
	}
	strcpy(TL_MENU_Data.list_str[TIME_Data.len], " .Add...");
}

void TL_MENU_Init(){
	TL_MENU_Data.first_line = 0;
	TIME_Init();
	for(int i = 0; i < NUMBER_OF_CHOICE; i++){
		for(int j = 0; j < 21; j++){
			TL_MENU_Data.list_str[i][j] = 0;
		}
	}
}

void TL_MENU_Set_State(uint8_t p_first_line){
	MENU_Data.state = TIME_LIST_MENU;
	MENU_Data.changed = 0;
	TL_MENU_Data.first_line = p_first_line;
	update_time_list();
	for(int i = 0; i < 3; i++){
		if((i + TL_MENU_Data.first_line) < (TIME_Data.len + 1) && (i + TL_MENU_Data.first_line) < NUMBER_OF_CHOICE){
			strcpy(time_list_menu_frame[i%3].str, TL_MENU_Data.list_str[i + TL_MENU_Data.first_line]);
			time_list_menu_frame[i%3].str[0] = i + '1';
		} else{
			strcpy(time_list_menu_frame[i%3].str, "");
		}
	}
}

void TL_MENU_Display(){
	LCD_Clear(MENU_Data.hlcd);
	LCD_Cursor_No_Blink(MENU_Data.hlcd);
	for(int i = 0; i < 4; i++){
		LCD_Set_Cursor(MENU_Data.hlcd, time_list_menu_frame[i].col, time_list_menu_frame[i].row);
		LCD_Write(MENU_Data.hlcd, time_list_menu_frame[i].str);
	}
}
