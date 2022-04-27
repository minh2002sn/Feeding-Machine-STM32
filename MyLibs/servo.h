#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_hal.h"

#define MIN_CCR_SERVO_VALUE 5
#define MAX_CCR_SERVO_VALUE 25

typedef enum{
	SERVO_OFF,
	SERVO_ON,
}SERVO_STATE_HandleTypeDef;

typedef struct{
	SERVO_STATE_HandleTypeDef state;
}SERVO_DATA_HandleTypeDef;

void SERVO_Init();
void SERVO_Set_State(SERVO_STATE_HandleTypeDef p_state);
void SERVO_Handle();

SERVO_DATA_HandleTypeDef SERVO_Data;

#endif
