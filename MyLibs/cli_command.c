#include "cli_command.h"

#define RESPONE_STR "OK\n"

extern UART_HandleTypeDef huart2;

void CLI_Execute(char *p_uart_buffer, uint8_t p_len){
	char *p_arg_value[10];
	uint8_t p_arg_num = 0;

//	HAL_UART_Transmit(&huart2, (uint8_t*)"0\n", 2, 500);
	if(MENU_Data.state == WIFI_SCANNING_MENU){
		if(WL_MENU_Is_Enable_Receiving_SSID() == 1 && WL_MENU_Data.num_of_wifi < 6 && strstr(p_uart_buffer, "SSID_START") == NULL && strstr(p_uart_buffer, "SSID_END") == NULL){
			strncpy(WL_MENU_Data.wifi_list[WL_MENU_Data.num_of_wifi++], p_uart_buffer, ((p_len > 18) ? 18 : p_len) + 1);
		}
		HAL_UART_Transmit(&huart2, (uint8_t*)RESPONE_STR, strlen(RESPONE_STR), 500);
	}
	char *token = strtok((char *)p_uart_buffer, " ");
	while(token != NULL){
		p_arg_value[p_arg_num++] = token;
		token = strtok(NULL, " ");
	}
//	HAL_UART_Transmit(&huart2, (uint8_t*)"2\n", 2, 500);

	if(strstr(p_arg_value[0], "TIME") != NULL){
		MAIN_MENU_Set_State_Time(atoi(p_arg_value[1]), atoi(p_arg_value[2]), atoi(p_arg_value[3]));
		CONTROL_Set_Time(atoi(p_arg_value[1]), atoi(p_arg_value[2]), atoi(p_arg_value[3]));
	} else if(strstr(p_arg_value[0], "WIFI") != NULL || strstr(p_arg_value[0], "SMART_CONFIG_RUN") != NULL){
		MAIN_MENU_Set_State_WiFi(p_arg_value, p_arg_num);
	} else if(strstr(p_arg_value[0], "SSID_START") != NULL){
		if(MENU_Data.state == WIFI_SCANNING_MENU){
			WL_MENU_Init();
			WL_MENU_Enable_Recieve_SSID();
		}
	} else if(strstr(p_arg_value[0], "SSID_END") != NULL){
		if(WL_MENU_Is_Enable_Receiving_SSID() == 1 && MENU_Data.state == WIFI_SCANNING_MENU){
			WL_MENU_Disable_Recieve_SSID();
			WL_MENU_Set_State(0);
		}
	}
}
