#include "cli_command.h"

extern UART_HandleTypeDef huart2;

void CLI_Execute(char *uart_buffer, uint8_t len){
	char *arg_value[10];
	uint8_t arg_num = 0;

	char *token = strtok((char *)uart_buffer, " ");
	while(token != NULL){
		arg_value[arg_num++] = token;
		token = strtok(NULL, " ");
	}

	if(strstr(arg_value[0], "TIME") != NULL){
		MAIN_MENU_Set_State_Time(atoi(arg_value[1]), atoi(arg_value[2]), atoi(arg_value[3]));
		CONTROL_Set_Time(atoi(arg_value[1]), atoi(arg_value[2]), atoi(arg_value[3]));
	} else if(strstr(arg_value[0], "WIFI") != NULL || strstr(arg_value[0], "SMART_CONFIG_RUN") != NULL){
		MAIN_MENU_Set_State_WiFi(arg_value, arg_num);
	}
}
