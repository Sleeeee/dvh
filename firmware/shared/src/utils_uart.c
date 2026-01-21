#include "utils_uart.h"

extern UART_HandleTypeDef huart1;

void Utils_UART_ReceiveEnter(void) {
  uint8_t rx;
  while (1) {
    if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK) {
      if (rx == '\r' || rx == '\n') return;
    }
  }
}
