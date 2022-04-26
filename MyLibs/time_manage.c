#include "time_manage.h"


//extern UART_HandleTypeDef huart1;

void TIME_Init(){
	TIME_Data.add = FIRST_PAGE_ADD + BYTE_PER_PAGE * 63;
	TIME_Data.len = 0;
	for(int i = 0; i < MAX_OPTIONS; i++){
		uint8_t data[5] = {};
		FLASH_Read(TIME_Data.add + i*5, data, 5);
		if(data[0] == 0xFF){
			TIME_Data.flash_data[i].day = 0xFF;
			TIME_Data.flash_data[i].hour = 0xFF;
			TIME_Data.flash_data[i].minute = 0xFF;
			TIME_Data.flash_data[i].mass = 0xFFFF;
		} else{
			TIME_Data.flash_data[i].day = data[0];
			TIME_Data.flash_data[i].hour = data[1];
			TIME_Data.flash_data[i].minute = data[2];
			TIME_Data.flash_data[i].mass = data[3] | (uint16_t)data[4] << 8;
			TIME_Data.len++;
		}
	}
}

void TIME_Add(uint8_t day, uint8_t hour, uint8_t minute, uint16_t mass){
	if(TIME_Data.len < MAX_OPTIONS){
		TIME_Data.flash_data[TIME_Data.len].day = day;
		TIME_Data.flash_data[TIME_Data.len].hour = hour;
		TIME_Data.flash_data[TIME_Data.len].minute = minute;
		TIME_Data.flash_data[TIME_Data.len].mass = mass;
		TIME_Data.len++;
		TIME_Sort();
		TIME_Store_To_Flash();
		CONTROL_Recheck_Time();
	}
}

void TIME_Change(uint8_t index, uint8_t day, uint8_t hour, uint8_t minute, uint16_t mass){
	TIME_Data.flash_data[index].day = day;
	TIME_Data.flash_data[index].hour = hour;
	TIME_Data.flash_data[index].minute = minute;
	TIME_Data.flash_data[index].mass = mass;
	TIME_Sort();
	TIME_Store_To_Flash();
}

void TIME_Delete(uint8_t index){
	if(TIME_Data.len >= 0){
		for(int i = index; i < TIME_Data.len - 1; i++){
			FLASH_DATA_HandleTypeDef temp = TIME_Data.flash_data[i];
			TIME_Data.flash_data[i] = TIME_Data.flash_data[i+1];
			TIME_Data.flash_data[i+1] = temp;
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
			uint16_t data_i = TIME_Data.flash_data[i].hour * 60 + TIME_Data.flash_data[i].minute;
			uint16_t data_j = TIME_Data.flash_data[j].hour * 60 + TIME_Data.flash_data[j].minute;
			if(data_i > data_j){
				FLASH_DATA_HandleTypeDef temp;
				temp = TIME_Data.flash_data[i];
				TIME_Data.flash_data[i] = TIME_Data.flash_data[j];
				TIME_Data.flash_data[j] = temp;
			}
		}
	}
}

void TIME_Store_To_Flash(){
	FLASH_Erase(63);
	uint8_t *data;
	data = (uint8_t *)malloc(TIME_Data.len * 5 * sizeof(uint8_t));
	for(int i = 0; i < TIME_Data.len; i++){
		*(data + i*5) = TIME_Data.flash_data[i].day;
		*(data + i*5 + 1) = TIME_Data.flash_data[i].hour;
		*(data + i*5 + 2) = TIME_Data.flash_data[i].minute;
		*(data + i*5 + 3) = TIME_Data.flash_data[i].mass;
		*(data + i*5 + 4) = TIME_Data.flash_data[i].mass >> 8;
	}
	FLASH_Write(63, data, TIME_Data.len * 5);
}

//void TIME_UART_Transmit(){
//	for(int i = 0; i < TIME_Data.len; i++){
//		uint8_t Tx_Buff[20];
//		sprintf(Tx_Buff, "%d %d %d %d\n", TIME_Data.flash_data[i].day, TIME_Data.flash_data[i].hour, TIME_Data.flash_data[i].minute, TIME_Data.flash_data[i].mass);
//		HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char *)Tx_Buff), 200);
//	}
//}
