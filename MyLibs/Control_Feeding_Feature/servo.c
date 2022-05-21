#include "servo.h"

extern TIM_HandleTypeDef htim4;

void SERVO_Init(){
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	htim4.Instance->CCR3 = MIN_CCR_SERVO_VALUE;
	SERVO_Data.state = SERVO_OFF;
}

void SERVO_Set_State(SERVO_STATE_HandleTypeDef p_state){
	SERVO_Data.state = p_state;
}

void SERVO_Handle(){
	if(SERVO_Data.state == SERVO_ON){
		htim4.Instance->CCR3 = MAX_CCR_SERVO_VALUE;
	} else{
		htim4.Instance->CCR3 = MIN_CCR_SERVO_VALUE;
	}
}
