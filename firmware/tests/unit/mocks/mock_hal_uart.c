#include "stm32f1xx_hal.h"
#include <string.h>

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
