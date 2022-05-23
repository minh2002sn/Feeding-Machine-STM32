#include "motor.h"

extern TIM_HandleTypeDef htim3;

void MOTOR_Init(MOTOR_MOVE_HandleTypeDef p_move){
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	MOTOR_Data.run_pwm_value = MIN_PWM_VALUE;
	MOTOR_Data.move = p_move;
	MOTOR_Data.state = MOTOR_OFF;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void MOTOR_Set_State(MOTOR_STATE_HandleTypdeDef p_state){
	MOTOR_Data.state = p_state;
}

void MOTOR_Set_PWM(uint8_t p_run_pwm_value){
	MOTOR_Data.run_pwm_value = p_run_pwm_value;
}

void MOTOR_Handle(){
	if(MOTOR_Data.state == MOTOR_ON){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, MOTOR_Data.move);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, !MOTOR_Data.move);
		htim3.Instance->CCR2 = MOTOR_Data.run_pwm_value;
	} else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		htim3.Instance->CCR2 = STOP_PWM_VALUE;
	}
}
