#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_hal.h"

#define MIN_CCR_SERVO_VALUE		500
#define MAX_CCR_SERVO_VALUE		2500
#define OPEN_CCR_SERVO_VALUE	800

typedef enum{
	SERVO_OFF,
	SERVO_ON,
}SERVO_STATE_HandleTypeDef;

typedef struct{
	SERVO_STATE_HandleTypeDef state;
	uint16_t CCR;
}SERVO_DATA_HandleTypeDef;

void SERVO_Init();
void SERVO_Set_State(SERVO_STATE_HandleTypeDef p_state);
void SERVO_Set_PWM(uint16_t p_ccr_register_value);
void SERVO_Handle();

extern SERVO_DATA_HandleTypeDef SERVO_Data;

#endif
