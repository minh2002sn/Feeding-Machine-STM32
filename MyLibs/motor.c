#include "motor.h"

extern TIM_HandleTypeDef htim3;

void MOTOR_Init(MOTOR_MOVE_HandleTypeDef move, uint8_t ccr_run_value){
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	MOTOR_Data.ccr_run_value = ccr_run_value;
	MOTOR_Data.move = move;
	MOTOR_Data.state = MOTOR_OFF;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void MOTOR_Set_State(MOTOR_STATE_HandleTypdeDef state){
	MOTOR_Data.state = state;
}

void MOTOR_Handle(){
	if(MOTOR_Data.state == MOTOR_ON){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, MOTOR_Data.move);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, !MOTOR_Data.move);
		htim3.Instance->CCR2 = MOTOR_Data.ccr_run_value;
	} else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		htim3.Instance->CCR2 = 0;
	}
}
