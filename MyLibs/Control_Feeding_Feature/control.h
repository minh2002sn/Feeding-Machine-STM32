#ifndef FEEDING_CONTROL_H
#define FEEDING_CONTROL_H

#include "time_manage.h"
#include "loadcell.h"
#include "motor.h"
#include "servo.h"
#include "mpu6050.h"
#include "stdint.h"
#include "string.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

typedef enum{
	MEASURING_BEFORE_FEEDING,
	WAITING_FOOD,
	THROWING_FOOD,
	MEASURING_AFTER_FEEDING,
} FEEDING_STATE_HandleTypeDef;

typedef enum{
	WAITING,
	FEEDING,
	FINDING_NEXT,
} CONTROL_STATE_HandleTypeDef;

typedef struct{
	uint8_t next_time_index;
	long start_mass;
	uint8_t hour;
	uint8_t minute;
	uint8_t day;
	CONTROL_STATE_HandleTypeDef state;
	FEEDING_STATE_HandleTypeDef feeding_state;
	uint8_t feeding_level;
	LC_HandleTypeDef *hlc1;
	LC_HandleTypeDef *hlc2;
	LC_HandleTypeDef *hlc3;
}CONTROL_DATA_HandleTypeDef;

void CONTROL_Init(LC_HandleTypeDef *p_hlc1, LC_HandleTypeDef *p_hlc2, LC_HandleTypeDef *p_hlc3);
void CONTROL_Handle();
void CONTROL_Recheck_Time();
void CONTROL_Set_Time(uint8_t p_hour, uint8_t p_minute, uint8_t p_day);
long get_mass();

extern CONTROL_DATA_HandleTypeDef CONTROL_Data;

#endif
