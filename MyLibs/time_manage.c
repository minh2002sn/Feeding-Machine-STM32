#include "time_manage.h"

#define DATA_FRAME_SIZE_BYTE 	(sizeof(FLASH_DATA_HandleTypeDef))	// in byte
#define DATA_FRAME_SIZE_BIT		(DATA_FRAME_SIZE_BYTE * 8)			// in bit
#define DAY_DATA_MASK			0x0000007F
#define HOUR_DATA_MASK			0x00000F80
#define MINUTE_DATA_MASK		0x0003F000
#define MASS_DATA_MASK			0xFFFC0000

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
		uint8_t t_data[DATA_FRAME_SIZE_BYTE] = {};
		FLASH_Read(TIME_Data.add + i*DATA_FRAME_SIZE_BYTE, t_data, DATA_FRAME_SIZE_BYTE);
		if(*((uint32_t *)t_data) == 0xFFFFFFFF){
			*(uint32_t *)(TIME_Data.flash_data + i) = 0xFFFFFFFF;
		} else{
			TIME_Data.flash_data[i] = *((FLASH_DATA_HandleTypeDef *)t_data);
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
		*((uint32_t *)(TIME_Data.flash_data + TIME_Data.len - 1)) = 0xFFFFFFFF;
		TIME_Data.len--;
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
	FLASH_DATA_HandleTypeDef *t_flash_data;
	t_flash_data = (FLASH_DATA_HandleTypeDef *)malloc(TIME_Data.len * DATA_FRAME_SIZE_BIT);
	for(int i = 0; i < TIME_Data.len; i++){
		*(t_flash_data + i) = *(TIME_Data.flash_data + i);
	}
	FLASH_Write(63, (uint8_t *)t_flash_data, TIME_Data.len * DATA_FRAME_SIZE_BYTE);
	free(t_flash_data);
}
