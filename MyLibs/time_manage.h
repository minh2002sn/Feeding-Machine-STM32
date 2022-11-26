#ifndef TIME_MANAGE_H
#define TIME_MANAGE_H

#define MAX_OPTIONS 14

#include "control.h"
#include "flash.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct{
	unsigned int day		: 7;
	unsigned int hour		: 5;
	unsigned int minute		: 6;
	unsigned int mass		: 14;
} __attribute__((packed)) FLASH_DATA_HandleTypeDef;

typedef struct{
	uint32_t add;
	FLASH_DATA_HandleTypeDef flash_data[MAX_OPTIONS];
	uint8_t len;
}TIME_DATA_HandleTypdeDef;

void TIME_Init();
void TIME_Add(uint8_t p_day, uint8_t p_hour, uint8_t p_minute, uint16_t p_mass);
void TIME_Change(uint8_t p_index, uint8_t p_day, uint8_t p_hour, uint8_t p_minute, uint16_t p_mass);
void TIME_Delete(uint8_t p_index);
void TIME_Sort();
void TIME_Store_To_Flash();
void TIME_UART_Display();

extern TIME_DATA_HandleTypdeDef TIME_Data;

#endif
