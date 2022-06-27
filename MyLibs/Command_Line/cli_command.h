#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include "menu.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "string.h"

void CLI_Execute(char *p_uart_buffer, uint8_t p_len);

#endif
