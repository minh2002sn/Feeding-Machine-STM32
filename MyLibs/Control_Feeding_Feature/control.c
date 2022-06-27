#include "control.h"

extern UART_HandleTypeDef huart2;
extern MPU6050_t mpu;
CONTROL_DATA_HandleTypeDef CONTROL_Data;

#define FEEDING_TIMEOUT			300000
#define FEEDING_LEVEL_TIMEOUT	20000
#define MEASURING_TIMEOUT		10000
#define MIN_MASS				5

#define ALPHA					1.0
#define BETA					1.0
#define GAMMA					1.0

#define ERROR_MASS 				3
#define KP_0					1.0
#define	KI_0					0.5
#define KD_0					10.0
#define KP_1					1.0
#define	KI_1					0.5
#define KD_1					10.0
#define KP_2					1.0
#define	KI_2					0.5
#define KD_2					10.0
static float I = 0;
static float last_error = 0.0;

static uint32_t feeding_timer = 0;
static uint32_t measure_timer = 0;
static uint8_t current_feeding_level;

void clean_pid_data(){
	I = 0.0;
	last_error = 0.0;
}

int pid_calculation(uint16_t p_mass, uint16_t p_des_mass){
	float KP = KP_0;
	float KI = KI_0;
	float KD = 0.5;
	float t_error = ((int16_t)p_des_mass - (int16_t)p_mass) * 1.0;
	float P = t_error;
	float D = t_error - last_error;

//	if(CONTROL_Data.start_mass > 4000){
//		KP = KP_2;
//		KI = KI_2;
//		KD = KD_2;
//	} else if(ONTROL_Data.start_mass > 2000){
//		KP = KP_1;
//		KI = KI_1;
//		KD = KD_1;
//	}

	if(t_error <= 15) KD = KD_0;

	if(last_error - t_error >= -0.1 && last_error - t_error <= 0.1){
		I += t_error;
	} else{
		if(I > 0){
			I -= t_error;
		} else{
			I = 0;
		}
	}

	last_error = t_error;
	int result = (KP * P) + (KI * I) + (KD * D);
//	char t_str[20] = {};
//	sprintf(t_str, "%d %d %d %d\n", (int)P, (int)I, (int)D, result);
//	HAL_UART_Transmit(&huart1, (uint8_t *)t_str, strlen(t_str), 500);
	return result;
}

long get_mass(){
	long t_mass_1 = (long)LC_Get_Mass(CONTROL_Data.hlc1);
	long t_mass_2 = (long)LC_Get_Mass(CONTROL_Data.hlc2);
	long t_mass_3 = (long)LC_Get_Mass(CONTROL_Data.hlc3);
	long t_result = (long)(ALPHA * t_mass_1 + BETA * t_mass_2 + GAMMA * t_mass_3);
//	uint8_t t_Tx_Buff[20] = {};
//	sprintf((char*)t_Tx_Buff, "%ld %ld %ld %ld\n", t_mass_1, t_mass_2, t_mass_3, t_result);
//	HAL_UART_Transmit(&huart2, t_Tx_Buff, strlen((char*)t_Tx_Buff), 500);
	return t_result;
}

static void wait(){
	uint16_t t_current_time = CONTROL_Data.hour * 60 + CONTROL_Data.minute;
	uint16_t t_feeding_time = TIME_Data.flash_data[CONTROL_Data.next_time_index].hour * 60 + TIME_Data.flash_data[CONTROL_Data.next_time_index].minute;
	if(t_current_time >= t_feeding_time && t_current_time - t_feeding_time <= 1){
		CONTROL_Data.state = FEEDING;
		CONTROL_Data.feeding_state = MEASURING_BEFORE_FEEDING;
		current_feeding_level = 1;
		clean_pid_data();
		feeding_timer = HAL_GetTick();
	}
	SERVO_Set_State(SERVO_OFF);
	MOTOR_Set_State(MOTOR_OFF);
}

static void feed(){
	static long temp_mass[3] = {};
	static uint8_t num_mass = 0;
	static uint32_t feeding_level_timer = 0;
	long t_current_mass = CONTROL_Data.start_mass - get_mass();
	long t_total_feeding_mass = TIME_Data.flash_data[CONTROL_Data.next_time_index].mass;
//	MPU6050_callback(&mpu);
	float pitch = 0;
	float roll = 0;
	if(current_feeding_level <= CONTROL_Data.feeding_level && pitch > -45 && pitch < 45 && roll > -45 && roll < 45 && HAL_GetTick() - feeding_timer < FEEDING_TIMEOUT){
		switch(CONTROL_Data.feeding_state){
			case MEASURING_BEFORE_FEEDING:
				SERVO_Set_State(SERVO_OFF);
				MOTOR_Set_State(MOTOR_OFF);
				if(HAL_GetTick() - measure_timer >= 1000){
					temp_mass[(num_mass) % 3] = get_mass();
					num_mass++;
					measure_timer = HAL_GetTick();
				}
				if(num_mass % 3 == 0){
					float t_delta_mass = (abs(temp_mass[0] - temp_mass[1]) + abs(temp_mass[1] - temp_mass[2])) / 2;
					if(t_delta_mass <= 1.1){
						long t_mass = (temp_mass[0] + temp_mass[1] + temp_mass[2]) / 3;
						if(current_feeding_level == 1)
							CONTROL_Data.start_mass = t_mass;
//						uint8_t t_Tx_Buff[20] = {};
//						sprintf((char*)t_Tx_Buff, "%ld,", t_mass);
//						HAL_UART_Transmit(&huart1, t_Tx_Buff, strlen((char*)t_Tx_Buff), 500);
						CONTROL_Data.feeding_state = WAITING_FOOD;
					}
				}
				break;
			case WAITING_FOOD:
				SERVO_Set_State(SERVO_ON);
				MOTOR_Set_State(MOTOR_OFF);
				long t_current_des_mass = (long)(t_total_feeding_mass * current_feeding_level / CONTROL_Data.feeding_level) - ERROR_MASS;
				if(t_current_mass > t_current_des_mass){
					CONTROL_Data.feeding_state = THROWING_FOOD;
					feeding_level_timer = HAL_GetTick();
				}
				// Set servo's PWM value
				uint16_t t_ccr_value = MIN_CCR_SERVO_VALUE + pid_calculation(t_current_mass, t_current_des_mass);
				if(t_ccr_value > OPEN_CCR_SERVO_VALUE)
					t_ccr_value = OPEN_CCR_SERVO_VALUE;
				else if(t_ccr_value < MIN_CCR_SERVO_VALUE)
					t_ccr_value = MIN_CCR_SERVO_VALUE;
				SERVO_Set_PWM(t_ccr_value);
				break;
			case THROWING_FOOD:
				SERVO_Set_State(SERVO_OFF);
				MOTOR_Set_PWM((MAX_PWM_VALUE - MIN_PWM_VALUE) * current_feeding_level / CONTROL_Data.feeding_level);
				MOTOR_Set_State(MOTOR_ON);
				if(HAL_GetTick() - feeding_level_timer > FEEDING_LEVEL_TIMEOUT){
					CONTROL_Data.feeding_state = MEASURING_AFTER_FEEDING;
					clean_pid_data();
				}
				break;
			case MEASURING_AFTER_FEEDING:
				SERVO_Set_State(SERVO_OFF);
				MOTOR_Set_State(MOTOR_OFF);
				if(HAL_GetTick() - measure_timer >= 1000){
					temp_mass[num_mass % 3] = get_mass();
					num_mass++;
					measure_timer = HAL_GetTick();
				}
				if(num_mass % 3 == 0){
					float t_delta_mass = (abs(temp_mass[0] - temp_mass[1]) + abs(temp_mass[1] - temp_mass[2])) / 2;
					if(t_delta_mass <= 1.1){
						long t_mass = (temp_mass[0] + temp_mass[1] + temp_mass[2]) / 3;
//						uint8_t t_Tx_Buff[20] = {};
//						sprintf((char*)t_Tx_Buff, "%ld,", t_mass);
//						HAL_UART_Transmit(&huart1, t_Tx_Buff, strlen((char*)t_Tx_Buff), 500);
//						if(current_feeding_level == CONTROL_Data.feeding_level)
//							HAL_UART_Transmit(&huart1, (uint8_t *)"\n", 1, 500);
						CONTROL_Data.feeding_state = MEASURING_BEFORE_FEEDING;
						measure_timer -= 1000;
						current_feeding_level++;
					}
				}
				break;
			default:
				break;
		}
	} else{
		CONTROL_Data.state = FINDING_NEXT;
	}
//	static uint32_t t_timer = 0;
//	if(HAL_GetTick() - t_timer >= 2000){
//		uint8_t Tx_Buff[50] = {};
//		sprintf((char*)Tx_Buff, "%d %f %f\n", CONTROL_Data.state, pitch, roll);
//		HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char*)Tx_Buff), 500);
//		t_timer = HAL_GetTick();
//	}
}

static void find_next(){
	if(TIME_Data.len != 0){
		for(int i = 0; i < TIME_Data.len; i++){
			uint16_t t_current_time = CONTROL_Data.hour * 60 + CONTROL_Data.minute;
			uint16_t t_feeding_time = TIME_Data.flash_data[i].hour * 60 + TIME_Data.flash_data[i].minute;
			if(t_current_time <= t_feeding_time && (CONTROL_Data.day & TIME_Data.flash_data[i].day) != 0){
				CONTROL_Data.next_time_index = i;
				CONTROL_Data.state = WAITING;
				break;
			}
		}
	}
	SERVO_Set_State(SERVO_OFF);
	MOTOR_Set_State(MOTOR_OFF);
}

void CONTROL_Init(LC_HandleTypeDef *p_hlc1, LC_HandleTypeDef *p_hlc2, LC_HandleTypeDef *p_hlc3){
	CONTROL_Data.state = FINDING_NEXT;
	CONTROL_Data.feeding_state = MEASURING_BEFORE_FEEDING;
	CONTROL_Data.feeding_level = 3;
	CONTROL_Data.hour = 0;
	CONTROL_Data.minute = 0;
	CONTROL_Data.day = 1;
	CONTROL_Data.hlc1 = p_hlc1;
	CONTROL_Data.hlc2 = p_hlc2;
	CONTROL_Data.hlc3 = p_hlc3;
	SERVO_Init();
	SERVO_Set_State(SERVO_OFF);
	MOTOR_Init(FORWARD);
	MOTOR_Set_State(MOTOR_OFF);
	MOTOR_Set_PWM(MIN_PWM_VALUE);
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
		case FINDING_NEXT:
			find_next();
			break;
		default:
			break;
	}
//	static uint32_t t_timer = 0;
//	if(HAL_GetTick() - t_timer >= 2000){
//		uint8_t Tx_Buff[50] = {};
//		sprintf((char*)Tx_Buff, "%d %d %d\n", CONTROL_Data.state, CONTROL_Data.day, TIME_Data.flash_data[CONTROL_Data.next_time_index].day);
//		HAL_UART_Transmit(&huart1, Tx_Buff, strlen((char*)Tx_Buff), 500);
//		t_timer = HAL_GetTick();
//	}
}

void CONTROL_Recheck_Time(){
	if(CONTROL_Data.state != FEEDING){
		CONTROL_Data.state = FINDING_NEXT;
	}
}

void CONTROL_Set_Time(uint8_t p_hour, uint8_t p_minute, uint8_t p_day){
	CONTROL_Data.hour = p_hour;
	CONTROL_Data.minute = p_minute;
	CONTROL_Data.day = p_day;
	CONTROL_Recheck_Time();
}

