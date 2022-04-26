#ifndef TIME_MANAGE_H
#define TIME_MANAGE_H

#define MAX_OPTIONS 14

#include "control.h"
#include "flash.h"
#include "stdlib.h"
#include "string.h"

typedef struct{
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint16_t mass;
}FLASH_DATA_HandleTypeDef;

typedef struct{
	uint32_t add;
	FLASH_DATA_HandleTypeDef flash_data[MAX_OPTIONS];
	uint8_t len;
}TIME_DATA_HandleTypdeDef;

void TIME_Init();
void TIME_Add(uint8_t day, uint8_t hour, uint8_t minute, uint16_t mass);
void TIME_Change(uint8_t index, uint8_t day, uint8_t hour, uint8_t minute, uint16_t mass);
void TIME_Delete(uint8_t index);
void TIME_Sort();
void TIME_Store_To_Flash();
//void TIME_UART_Transmit();

TIME_DATA_HandleTypdeDef TIME_Data;

#endif
