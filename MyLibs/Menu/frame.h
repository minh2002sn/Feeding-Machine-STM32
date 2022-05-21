#ifndef FRAME_H
#define FRAME_H

#include "stdint.h"

typedef struct{
	uint8_t col;
	uint8_t row;
	char str[21];
}FRAME_HandleTypeDef;

#endif
