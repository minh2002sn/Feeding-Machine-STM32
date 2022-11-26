#ifndef UART_FRAME_H
#define UART_FRAME_H

#include "min.h"
#include <stdlib.h>
#include "string.h"
#include "main_menu.h"

// STM enum
typedef enum{
	COMMAND,
	RESPONSE,
	WIFI_CONNECTION_DATA,
	WIFI_SSID,
	WIFI_STATUS,
	REAL_TIME,
} PACKAGE_TYPE_t;

typedef enum{
	NACK,
	ACK,
} RESPONSE_t;

typedef enum{
	GET_CONNECTION,
	GET_REAL_TIME,
	GET_NETWORKS,
	START_SENDING_SSID,
	END_SENDING_SSID,
} COMMAND_t;

// ESP enum
//typedef enum{
//	WIFI_CONNECTED,
//	WIFI_DISCONNECTED,
//	WIFI_CONNECTING,
//} WIFI_STATUS_t;

typedef struct{
	unsigned int day		: 7;
	unsigned int hour		: 5;
	unsigned int minute		: 12;
} __attribute__((packed)) TIME_TRANSMIT_DATA_FRAME_t;

void UART_Send_Command(COMMAND_t p_commnad);
void UART_Send_Response(RESPONSE_t p_repsonse);
void UART_Send_WiFi_Data(uint8_t p_ssid_number, uint8_t* p_pass, uint8_t p_pass_length);
void UART_Send_WiFi_Status(WIFI_STATE_HandleTypeDef p_status);
void UART_Send_Real_Time(uint8_t p_day, uint8_t p_hour, uint8_t p_minute);
void UART_Send_SSID(const char *p_ssid);
void UART_Start_Sending_SSID();
void UART_End_Sending_SSID();

#endif
