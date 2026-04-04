#include "stm32f1xx_hal.h"
#include <string.h>

// General HAL mocks
uint32_t HAL_GetTick(void) {
  return mock_tick_counter++;
}

// UART mocks
UART_HandleTypeDef huart1;

char SPY_UART_Buffer[128];
int SPY_UART_CallCount = 0;

void SPY_UART_Clear(void) {
  memset(SPY_UART_Buffer, 0, sizeof(SPY_UART_Buffer));
  SPY_UART_CallCount = 0;
}

HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  strncat(SPY_UART_Buffer, (char*)pData, Size);
  SPY_UART_CallCount++;
  return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  return HAL_OK;
}

// I2C mocks
I2C_HandleTypeDef hi2c2;

uint8_t SPY_EEPROM_Buffer[VIRTUAL_EEPROM_SIZE];
int SPY_I2C_Write_CallCount = 0;
HAL_StatusTypeDef SPY_I2C_Mock_Status = HAL_OK;

void SPY_I2C_Clear(void) {
  memset(SPY_EEPROM_Buffer, 0xff, VIRTUAL_EEPROM_SIZE); // Blank EEPROM is filled with 1s
  SPY_I2C_Write_CallCount = 0;
  SPY_I2C_Mock_Status = HAL_OK;
  mock_tick_counter = 0;
}

HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout) {
  return SPY_I2C_Mock_Status;
}

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  if (SPY_I2C_Mock_Status != HAL_OK) return SPY_I2C_Mock_Status;
  if (MemAddress + Size > VIRTUAL_EEPROM_SIZE) return HAL_ERROR;

  memcpy(pData, &SPY_EEPROM_Buffer[MemAddress], Size);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  SPY_I2C_Write_CallCount++; 

  if (SPY_I2C_Mock_Status != HAL_OK) return SPY_I2C_Mock_Status;
  if (MemAddress + Size > VIRTUAL_EEPROM_SIZE) return HAL_ERROR;

  memcpy(&SPY_EEPROM_Buffer[MemAddress], pData, Size);
  return HAL_OK;
}
