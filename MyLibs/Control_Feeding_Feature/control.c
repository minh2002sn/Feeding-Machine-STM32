#include "control.h"

extern UART_HandleTypeDef huart1;
extern MPU6050_t mpu;

#define FEEDING_TIMEOUT		60000
#define MIN_MASS			5

static uint32_t feeding_timeout = 0;


static float alpha = 1.0;
static float beta = 1.0;
static float gamma = 1.0;

long get_mass(){
	long t_mass_1 = (long)LC_Get_Mass(CONTROL_Data.hlc1);
	long t_mass_2 = (long)LC_Get_Mass(CONTROL_Data.hlc2);
	long t_mass_3 = (long)LC_Get_Mass(CONTROL_Data.hlc3);
//	uint8_t t_Tx_Buff[20] = {};
//	sprintf((char*)t_Tx_Buff, "%ld %ld %ld ", t_mass_1, t_mass_2, t_mass_3);
//	HAL_UART_Transmit(&huart1, t_Tx_Buff, strlen((char*)t_Tx_Buff), 500);
	return (long)(alpha * t_mass_1 + beta * t_mass_2 + gamma * t_mass_3);
}

static void wait(){
	uint16_t t_current_time = CONTROL_Data.hour * 60 + CONTROL_Data.minute;
	uint16_t t_feeding_time = TIME_Data.flash_data[CONTROL_Data.next_time_index].hour * 60 + TIME_Data.flash_data[CONTROL_Data.next_time_index].minute;
	if(t_current_time >= t_feeding_time && t_current_time - t_feeding_time <= 1){
		CONTROL_Data.start_mass = get_mass();
		CONTROL_Data.state = FEEDING;
		feeding_timeout = HAL_GetTick();
//		uint8_t Tx_Buff[50] = {};
//		sprintf((char*)Tx_Buff, "%ld %d\n", CONTROL_Data.start_mass, TIME_Data.flash_data[CONTROL_Data.next_time_index].mass);
//		HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char*)Tx_Buff), 500);
	}
}

static void feed(){
	long t_mass = get_mass();
//	MPU6050_callback(&mpu);
	long t_feeding_mass = TIME_Data.flash_data[CONTROL_Data.next_time_index].mass;
	if(CONTROL_Data.start_mass - t_mass < t_feeding_mass && HAL_GetTick() - feeding_timeout < FEEDING_TIMEOUT){
		SERVO_Set_State(SERVO_ON);
		float t_pitch = 0;
//		uint8_t Tx_Buff[50] = {};
//		sprintf((char*)Tx_Buff, "%ld\n", t_mass);
//		HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char*)Tx_Buff), 500);
		if(t_pitch < 30){
			MOTOR_Set_State(MOTOR_ON, MAX_PWM_VALUE);
		} else{
			MOTOR_Set_State(MOTOR_OFF, MIN_PWM_VALUE);
		}
	} else{
		SERVO_Set_State(SERVO_OFF);
		MOTOR_Set_State(MOTOR_OFF, MIN_PWM_VALUE);
		CONTROL_Data.state = FIND_NEXT;
//		uint8_t Tx_Buff[50] = {};
//		sprintf((char*)Tx_Buff, "%ld\n", t_mass);
//		HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char*)Tx_Buff), 500);
	}
}

static void find_next(){
	if(TIME_Data.len != 0){
		for(int i = 0; i < TIME_Data.len; i++){
			uint16_t t_current_time = CONTROL_Data.hour * 60 + CONTROL_Data.minute;
			uint16_t t_feeding_time = TIME_Data.flash_data[i].hour * 60 + TIME_Data.flash_data[i].minute;
			if(t_current_time < t_feeding_time && (CONTROL_Data.day | TIME_Data.flash_data[i].day) != 0){
				CONTROL_Data.next_time_index = i;
				CONTROL_Data.state = WAITING;
				return;
			}
//			uint8_t Tx_Buff[50] = {};
//			sprintf((char*)Tx_Buff, "%d %d\n", t_current_time, t_feeding_time);
//			HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char*)Tx_Buff), 500);
		}
	}
}

void CONTROL_Init(LC_HandleTypeDef *p_hlc1, LC_HandleTypeDef *p_hlc2, LC_HandleTypeDef *p_hlc3){
	CONTROL_Data.state = FIND_NEXT;
	CONTROL_Data.hlc1 = p_hlc1;
	CONTROL_Data.hlc2 = p_hlc2;
	CONTROL_Data.hlc3 = p_hlc3;
	SERVO_Init();
	SERVO_Set_State(SERVO_OFF);
	MOTOR_Init(FORWARD);
	MOTOR_Set_State(MOTOR_OFF, MIN_PWM_VALUE);
}

void CONTROL_Handle(){
	SERVO_Handle();
	MOTOR_Handle();
	switch(CONTROL_Data.state){
		case WAITING:
			wait();
			break;
		case FEEDING:
			feed();
			break;
		case FIND_NEXT:
			find_next();
			break;
	}
	static uint32_t t_timer = 0;
	if(HAL_GetTick() - t_timer >= 2000){
		uint8_t Tx_Buff[50] = {};
		sprintf((char*)Tx_Buff, "%d %d %d\n", CONTROL_Data.state, CONTROL_Data.hour, TIME_Data.flash_data[CONTROL_Data.next_time_index].hour);
		HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char*)Tx_Buff), 500);
		t_timer = HAL_GetTick();
	}
}

void CONTROL_Recheck_Time(){
	if(CONTROL_Data.state == FEEDING){
		SERVO_Set_State(SERVO_OFF);
		MOTOR_Set_State(MOTOR_OFF, MIN_PWM_VALUE);
	}
	CONTROL_Data.state = FIND_NEXT;
}

void CONTROL_Set_Time(uint8_t p_hour, uint8_t p_minute, uint8_t p_day){
	CONTROL_Data.hour = p_hour;
	CONTROL_Data.minute = p_minute;
	CONTROL_Data.day = p_day;
}

