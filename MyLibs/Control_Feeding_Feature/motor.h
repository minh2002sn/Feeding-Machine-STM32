#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f1xx_hal.h"
#include "stdint.h"

#define MIN_PWM_VALUE 0
#define MAX_PWM_VALUE 200

typedef enum{
	BACKWARD,
	FORWARD,
}MOTOR_MOVE_HandleTypeDef;

typedef enum{
	MOTOR_OFF,
	MOTOR_ON,
}MOTOR_STATE_HandleTypdeDef;

typedef struct{
	uint8_t run_pwm_value;
	MOTOR_MOVE_HandleTypeDef move;
	MOTOR_STATE_HandleTypdeDef state;
}MOTOR_DATA_HandleTypeDef;

void MOTOR_Init(MOTOR_MOVE_HandleTypeDef p_move);
void MOTOR_Set_State(MOTOR_STATE_HandleTypdeDef p_state, uint8_t p_run_pwm_value);
void MOTOR_Handle();

MOTOR_DATA_HandleTypeDef MOTOR_Data;

#endif
