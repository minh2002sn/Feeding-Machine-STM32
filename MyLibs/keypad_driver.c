#include "keypad_driver.h"

extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart1;

void KeypadPressingCallback(uint8_t p_key){
	switch(MENU_Data.state){
		case MAIN_MENU:
			if(p_key == '1'){
				TL_MENU_Set_State(0);
			} else if(p_key == '2'){
				SM_MENU_Set_State();	// This function is in menu.c
			}
			break;
		case TIME_LIST_MENU:
			if(p_key >= '1' && p_key <= '3'){
				if((TL_MENU_Data.first_line + p_key - '1') == TIME_Data.len){
					ST_MENU_Set_State();
				} else if ((p_key - '1') < TIME_Data.len){
					DELETE_MENU_Set_State(TL_MENU_Data.first_line + p_key - '1');
				}
			} else if(p_key == '*'){
				if((TL_MENU_Data.first_line - 3) >= 0){
					TL_MENU_Set_State(TL_MENU_Data.first_line - 3);
				} else{
					MAIN_MENU_Set_State();
				}
			} else if(p_key == '#'){
				if((TL_MENU_Data.first_line + 3) < TIME_Data.len+1){
					TL_MENU_Set_State(TL_MENU_Data.first_line + 3);
				}
			}
			break;
		case SET_TIME_MENU:
			if((p_key >= '0' && p_key <= '9') || p_key == 'C'){
				ST_MENU_Set_State_NumKey(p_key);
			} else if(p_key == '*' || p_key == '#'){
				ST_MENU_Set_State_SigKey(p_key);
			} else if(p_key == 'D'){
				TL_MENU_Set_State(0);
			}
			break;
		case DELETE_TIME_MENU:
			if(p_key == 'D'){
				DELETE_MENU_Set_State_Delete();
			} else if(p_key == '*'){
				TL_MENU_Set_State(TL_MENU_Data.first_line);
			}
			break;
		case SMART_CONFIG_MENU:
			if(p_key == '#'){
				uint8_t Tx_Buff[] = "SMART_CONFIG\n";
				HAL_UART_Transmit(&huart2, Tx_Buff, 13, 500);
				MAIN_MENU_Set_State();
			} else if(p_key == '*'){
				MAIN_MENU_Set_State();
			}
		default:
			break;
	}
//	uint8_t Tx_Buff[2] = {p_key, '\n'};
//	HAL_UART_Transmit(&huart1, Tx_Buff, 2, 500);
}

void KEYPAD_DRIVER_Handle(){
	Keypad_Handle();
}
