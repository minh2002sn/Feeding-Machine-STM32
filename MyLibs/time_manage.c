#include "time_manage.h"

#define DATA_FRAME_SIZE 6 // in byte

extern UART_HandleTypeDef huart2;
TIME_DATA_HandleTypdeDef TIME_Data;

void TIME_UART_Display(){
	if(TIME_Data.len == 0){
		HAL_UART_Transmit(&huart2, (uint8_t *)"No data\n", 8, 1000);
	} else{
		for(int i = 0; i < TIME_Data.len; i++){
			char Tx_Buffer[30] = {};
			sprintf(Tx_Buffer, "%d, %dh%dm: %d (g)\n", TIME_Data.flash_data[i].day, TIME_Data.flash_data[i].hour,
					TIME_Data.flash_data[i].minute, TIME_Data.flash_data[i].mass);
			HAL_UART_Transmit(&huart2, (uint8_t *)Tx_Buffer, strlen(Tx_Buffer), 1000);
		}
	}
}

void TIME_Init(){
	TIME_Data.add = FIRST_PAGE_ADD + BYTE_PER_PAGE * 63;
	TIME_Data.len = 0;
	for(int i = 0; i < MAX_OPTIONS; i++){
		uint8_t t_data[DATA_FRAME_SIZE] = {};
		FLASH_Read(TIME_Data.add + i*DATA_FRAME_SIZE, t_data, DATA_FRAME_SIZE);
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
//		CONTROL_Recheck_Time();
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
		TIME_Data.flash_data[TIME_Data.len - 1].mass = 0xFFFF;
		TIME_Data.len--;
		TIME_Store_To_Flash();
//		CONTROL_Recheck_Time();
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
	uint8_t *t_data;
	t_data = (uint8_t *)malloc(TIME_Data.len * DATA_FRAME_SIZE * sizeof(uint8_t));
	for(int i = 0; i < TIME_Data.len; i++){
		*(t_data + i * DATA_FRAME_SIZE) = TIME_Data.flash_data[i].day;
		*(t_data + i * DATA_FRAME_SIZE + 1) = TIME_Data.flash_data[i].hour;
		*(t_data + i * DATA_FRAME_SIZE + 2) = TIME_Data.flash_data[i].minute;
		*(t_data + i * DATA_FRAME_SIZE + 3) = TIME_Data.flash_data[i].mass;
		*(t_data + i * DATA_FRAME_SIZE + 4) = TIME_Data.flash_data[i].mass >> 8;
		*(t_data + i * DATA_FRAME_SIZE + 5) = 0xFF;
	}
	FLASH_Write(63, t_data, TIME_Data.len * DATA_FRAME_SIZE);
}
