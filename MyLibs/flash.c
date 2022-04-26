#include "flash.h"

void FLASH_Erase(uint32_t p_page){
	HAL_FLASH_Unlock();
	uint32_t t_PageError;
	FLASH_EraseInitTypeDef eraseInit;
	eraseInit.Banks = 1;
	eraseInit.NbPages = 1;
	eraseInit.PageAddress = FIRST_PAGE_ADD + BYTE_PER_PAGE * (p_page);
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	HAL_FLASHEx_Erase(&eraseInit, &t_PageError);
	HAL_FLASH_Lock();
}

void FLASH_Write(uint32_t p_page, uint8_t *p_data, uint16_t p_len){
	uint32_t t_add = FIRST_PAGE_ADD + BYTE_PER_PAGE * (p_page);
	HAL_FLASH_Unlock();
	for(int i = 0; i < p_len; i += 2){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, t_add + i, p_data[i] | (uint16_t)((i+1 >= p_len) ? 0xFF : p_data[i+1]) << 8);
	}
	HAL_FLASH_Lock();
}

void FLASH_Read(uint32_t add, uint8_t *p_data, uint16_t p_len){
	for(int i = 0; i < p_len; i += 2){
		uint16_t t_data = *(volatile uint32_t *)(add + i);
		p_data[i] = t_data;
		p_data[i+1] = t_data >> 8;
	}
}
