#include "time_manage.h"


//extern UART_HandleTypeDef huart1;

void TIME_Init(){
	TIME_Data.add = FIRST_PAGE_ADD + BYTE_PER_PAGE * 63;
	TIME_Data.len = 0;
	for(int i = 0; i < MAX_OPTIONS; i++){
		uint8_t t_data[5] = {};
		FLASH_Read(TIME_Data.add + i*5, t_data, 5);
		if(t_data[0] == 0xFF){
			TIME_Data.flash_data[i].day = 0xFF;
			TIME_Data.flash_data[i].hour = 0xFF;
			TIME_Data.flash_data[i].minute = 0xFF;
			TIME_Data.flash_data[i].mass = 0xFFFF;
		} else{
			TIME_Data.flash_data[i].day = t_data[0];
			TIME_Data.flash_data[i].hour = t_data[1];
			TIME_Data.flash_data[i].minute = t_data[2];
			TIME_Data.flash_data[i].mass = t_data[3] | (uint16_t)t_data[4] << 8;
			TIME_Data.len++;
		}
	}
}

void TIME_Add(uint8_t p_day, uint8_t p_hour, uint8_t p_minute, uint16_t p_mass){
	if(TIME_Data.len < MAX_OPTIONS){
		TIME_Data.flash_data[TIME_Data.len].day = p_day;
		TIME_Data.flash_data[TIME_Data.len].hour = p_hour;
		TIME_Data.flash_data[TIME_Data.len].minute = p_minute;
		TIME_Data.flash_data[TIME_Data.len].mass = p_mass;
		TIME_Data.len++;
		TIME_Sort();
		TIME_Store_To_Flash();
		CONTROL_Recheck_Time();
	}
}

void TIME_Change(uint8_t p_index, uint8_t p_day, uint8_t p_hour, uint8_t p_minute, uint16_t p_mass){
	TIME_Data.flash_data[p_index].day = p_day;
	TIME_Data.flash_data[p_index].hour = p_hour;
	TIME_Data.flash_data[p_index].minute = p_minute;
	TIME_Data.flash_data[p_index].mass = p_mass;
	TIME_Sort();
	TIME_Store_To_Flash();
}

void TIME_Delete(uint8_t p_index){
	if(TIME_Data.len >= 0){
		for(int i = p_index; i < TIME_Data.len - 1; i++){
			FLASH_DATA_HandleTypeDef t_temp = TIME_Data.flash_data[i];
			TIME_Data.flash_data[i] = TIME_Data.flash_data[i+1];
			TIME_Data.flash_data[i+1] = t_temp;
		}
		TIME_Data.flash_data[TIME_Data.len - 1].day = 0xFF;
		TIME_Data.flash_data[TIME_Data.len - 1].hour = 0xFF;
		TIME_Data.flash_data[TIME_Data.len - 1].minute = 0xFF;
		TIME_Data.flash_data[TIME_Data.len - 1].mass = 0xFF;
		TIME_Data.len--;
		TIME_Store_To_Flash();
		CONTROL_Recheck_Time();
	}
}

void TIME_Sort(){
	for(int i = 0; i < TIME_Data.len; i++){
		for(int j = i + 1; j < TIME_Data.len; j++){
			uint16_t t_data_i = TIME_Data.flash_data[i].hour * 60 + TIME_Data.flash_data[i].minute;
			uint16_t t_data_j = TIME_Data.flash_data[j].hour * 60 + TIME_Data.flash_data[j].minute;
			if(t_data_i > t_data_j){
				FLASH_DATA_HandleTypeDef t_temp;
				t_temp = TIME_Data.flash_data[i];
				TIME_Data.flash_data[i] = TIME_Data.flash_data[j];
				TIME_Data.flash_data[j] = t_temp;
			}
		}
	}
}

void TIME_Store_To_Flash(){
	FLASH_Erase(63);
	uint8_t *t_data;
	t_data = (uint8_t *)malloc(TIME_Data.len * 5 * sizeof(uint8_t));
	for(int i = 0; i < TIME_Data.len; i++){
		*(t_data + i*5) = TIME_Data.flash_data[i].day;
		*(t_data + i*5 + 1) = TIME_Data.flash_data[i].hour;
		*(t_data + i*5 + 2) = TIME_Data.flash_data[i].minute;
		*(t_data + i*5 + 3) = TIME_Data.flash_data[i].mass;
		*(t_data + i*5 + 4) = TIME_Data.flash_data[i].mass >> 8;
	}
	FLASH_Write(63, t_data, TIME_Data.len * 5);
}
