#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "menu.h"

void CLI_Execute(char *p_uart_buffer, uint8_t p_len);

#endif