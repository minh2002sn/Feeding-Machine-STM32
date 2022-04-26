#include "cli_command.h"

extern UART_HandleTypeDef huart2;

void CLI_Execute(char *p_uart_buffer, uint8_t p_len){
	char *p_arg_value[10];
	uint8_t p_arg_num = 0;

	char *token = strtok((char *)p_uart_buffer, " ");
	while(token != NULL){
		p_arg_value[p_arg_num++] = token;
		token = strtok(NULL, " ");
	}

	if(strstr(p_arg_value[0], "TIME") != NULL){
		MAIN_MENU_Set_State_Time(atoi(p_arg_value[1]), atoi(p_arg_value[2]), atoi(p_arg_value[3]));
		CONTROL_Set_Time(atoi(p_arg_value[1]), atoi(p_arg_value[2]), atoi(p_arg_value[3]));
	} else if(strstr(p_arg_value[0], "WIFI") != NULL || strstr(p_arg_value[0], "SMART_CONFIG_RUN") != NULL){
		MAIN_MENU_Set_State_WiFi(p_arg_value, p_arg_num);
	}
}
