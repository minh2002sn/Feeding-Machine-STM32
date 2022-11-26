#include "min.h"
#include "main.h"
#include "uart_frame.h"
#include "menu.h"

#define DAY_DATA_MASK		0x0000007F
#define HOUR_DATA_MASK		0x00000F80
#define MINUTE_DATA_MASK	0x0003F000

uint32_t MIN_Get_Tick_ms(){
	return HAL_GetTick();
}

uint16_t MIN_Transmit_Space(uint8_t p_port){
	return 256;
}

void MIN_Transmit_Byte(uint8_t p_port, uint8_t p_byte){
#ifdef __STM32F0xx_HAL_H
	USART2->TDR = p_byte;
	while(!(USART2->ISR & USART_ISR_TC));
#else
	USART2->DR = p_byte;
	while(!(USART2->SR & USART_SR_TC));
#endif
}

void MIN_Start_Transmit(uint8_t p_port){

}

void MIN_Finish_Transmit(uint8_t p_port){

}

void MIN_Application_Handle(uint8_t p_control_id, uint8_t const *p_payload, uint32_t p_payload_length, uint8_t p_port){
	switch(p_control_id){
		case COMMAND:
			if(p_payload[0] == START_SENDING_SSID){
				if(MENU_Data.state == WIFI_SCANNING_MENU && !WL_MENU_Is_Enable_Receiving_SSID()){
					WL_MENU_Enable_Recieve_SSID();
					UART_Send_Response(ACK);
				}
			} else if(p_payload[0] == END_SENDING_SSID){
				if(MENU_Data.state == WIFI_SCANNING_MENU && WL_MENU_Is_Enable_Receiving_SSID()){
					WL_MENU_Disable_Recieve_SSID();
					WL_MENU_Set_State(0);
					UART_Send_Response(ACK);
				}
			}
			break;
		case WIFI_SSID:
			if(MENU_Data.state == WIFI_SCANNING_MENU){
				uint8_t t_pass_length = p_payload_length - 2;
				strncpy(WL_MENU_Data.wifi_list[WL_MENU_Data.num_of_wifi++], (char *)(p_payload + 1), ((t_pass_length > 18) ? 18 : t_pass_length) + 1);
				UART_Send_Response(ACK);
			}
			break;
		case WIFI_STATUS:
			MAIN_MENU_Set_State_WiFi((WIFI_STATE_HandleTypeDef)p_payload[0]);
			break;
		case REAL_TIME:
		{
			uint32_t t_data = (uint32_t)(p_payload[2] << 16) | (uint32_t)(p_payload[1] << 8) | p_payload[0];
			TIME_TRANSMIT_DATA_FRAME_t t_time_data_frame = {t_data & DAY_DATA_MASK, (t_data & HOUR_DATA_MASK) >> 7, (t_data & MINUTE_DATA_MASK) >> 12};
			MAIN_MENU_Set_State_Time(t_time_data_frame.hour, t_time_data_frame.minute, t_time_data_frame.day);
			CONTROL_Set_Time(t_time_data_frame.hour, t_time_data_frame.minute, t_time_data_frame.day);
			break;
		}
	}
}

