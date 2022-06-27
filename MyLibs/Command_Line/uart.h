#ifndef UART_H
#define UART_H

//#include "read_time.h"
#include "stdint.h"
#include "stm32f0xx_hal.h"
#include "cli_command.h"

void UART_Init();
void UART_Handle();
void UART_Receive(uint8_t P_Rx_Buffer);

#endif
