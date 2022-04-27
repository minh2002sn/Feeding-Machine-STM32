#ifndef LOADCELL_H
#define LOADCELL_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

#define NUMBER_OF_SAMPLE 100

typedef enum{
	NO_CALIBRATE,
	GETTING_X0,
	WAITING_SAMPLE_MASS,
	GETTING_X1,
	CALIBRATED,
}LC_CALIB_HandleTypeDef;

typedef struct{
	GPIO_TypeDef *DT_GPIOx;
	uint16_t DT_GPIO_Pin;
	GPIO_TypeDef *CLK_GPIOx;
	uint16_t CLK_GPIO_Pin;
	LC_CALIB_HandleTypeDef calib_state;
	float a;
	float b;
}LC_HandleTypeDef;

void LC_Init(LC_HandleTypeDef *p_hlc, GPIO_TypeDef *p_DT_GPIOx, uint16_t p_DT_GPIO_Pin, GPIO_TypeDef *p_CLK_GPIOx, uint16_t p_CLK_GPIO_Pin);
long LC_Read(LC_HandleTypeDef *p_hlc);
void LC_Calibration(LC_HandleTypeDef *p_hlc);
float LC_Get_Mass(LC_HandleTypeDef *p_hlc);

#endif
