#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f1xx_hal.h"
#include "stdint.h"

#define MIN_CCR_MOTOR_VALUE 0
#define MAX_CCR_MOTOR_VALUE 200

typedef enum{
	BACKWARD,
	FORWARD,
}MOTOR_MOVE_HandleTypeDef;

typedef enum{
	MOTOR_OFF,
	MOTOR_ON,
}MOTOR_STATE_HandleTypdeDef;

typedef struct{
	uint8_t ccr_run_value;
	MOTOR_MOVE_HandleTypeDef move;
	MOTOR_STATE_HandleTypdeDef state;
}MOTOR_DATA_HandleTypeDef;

void MOTOR_Init(MOTOR_MOVE_HandleTypeDef move, uint8_t ccr_run_value);
void MOTOR_Set_State(MOTOR_STATE_HandleTypdeDef state);
void MOTOR_Handle();

MOTOR_DATA_HandleTypeDef MOTOR_Data;

#endif
