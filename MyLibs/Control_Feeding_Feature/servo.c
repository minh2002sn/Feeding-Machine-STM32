#include "servo.h"

extern TIM_HandleTypeDef htim16;
SERVO_DATA_HandleTypeDef SERVO_Data;

void SERVO_Init(){
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	htim16.Instance->CCR1 = MIN_CCR_SERVO_VALUE;
	SERVO_Data.state = SERVO_OFF;
	SERVO_Data.CCR = MIN_CCR_SERVO_VALUE;
}

void SERVO_Set_State(SERVO_STATE_HandleTypeDef p_state){
	SERVO_Data.state = p_state;
}

void SERVO_Set_PWM(uint16_t p_ccr_register_value){
	SERVO_Data.CCR = p_ccr_register_value;
}

void SERVO_Handle(){
	if(SERVO_Data.state == SERVO_ON){
		htim16.Instance->CCR1 = SERVO_Data.CCR;
	} else{
		SERVO_Data.CCR = MIN_CCR_SERVO_VALUE;
		htim16.Instance->CCR1 = SERVO_Data.CCR;
	}
}
