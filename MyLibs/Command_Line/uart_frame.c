#include "uart_frame.h"

extern MIN_HandleTypeDef hmin;

void UART_Send_Command(COMMAND_t p_commnand){
	MIN_Send_Data(&hmin, COMMAND, (uint8_t *)&p_commnand, 1);
}

void UART_Send_Response(RESPONSE_t p_response){
	MIN_Send_Data(&hmin, RESPONSE, (uint8_t *)&p_response, 1);
}

void UART_Send_WiFi_Data(uint8_t p_ssid_number, uint8_t* p_pass, uint8_t p_pass_length){
	uint8_t *t_data_frame;
	t_data_frame = (uint8_t *)malloc((p_pass_length + 2) * 8);
	*t_data_frame = p_ssid_number;
	for(int i = 0; i < p_pass_length; i++){
		*(t_data_frame + i + 1) = p_pass[i];
	}
	*(t_data_frame + p_pass_length + 1) = '\0';
	MIN_Send_Data(&hmin, WIFI_CONNECTION_DATA, t_data_frame, p_pass_length + 1);
	free(t_data_frame);
}

void UART_Send_WiFi_Status(WIFI_STATE_HandleTypeDef p_status){
	MIN_Send_Data(&hmin, WIFI_STATUS, (uint8_t *)&p_status, 1);
}

void UART_Send_Real_Time(uint8_t p_day, uint8_t p_hour, uint8_t p_minute){
	TIME_TRANSMIT_DATA_FRAME_t t_data_frame = {p_day, p_hour, p_minute};
	MIN_Send_Data(&hmin, REAL_TIME, (uint8_t *)&t_data_frame, sizeof(t_data_frame));
}

void UART_Send_SSID(const char *p_ssid){
	MIN_Send_Data(&hmin, WIFI_SSID, (uint8_t *)p_ssid, sizeof(p_ssid)); // Send ssid string including '\0'
}

void UART_Start_Sending_SSID(){
	uint8_t t_data = START_SENDING_SSID;
	MIN_Send_Data(&hmin, COMMAND, &t_data, 1);
}

void UART_End_Sending_SSID(){
	uint8_t t_data = END_SENDING_SSID;
	MIN_Send_Data(&hmin, COMMAND, &t_data, 1);
}
