#include "utils_uart.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

void Utils_UART_Writeline(const char* text) {
  HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), 1000);
}

void Utils_UART_ReceiveEnter(void) {
  uint8_t rx;
  while (1) {
    if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK) {
      if (rx == '\r' || rx == '\n') {
        // Clear terminal to prevent recurring format issues
        HAL_UART_Transmit(&huart1, (uint8_t*)"\033[2J\033[H", 7, 10);
        return;
      }
    }
  }
}

int Utils_UART_Readchar(char* buffer, uint8_t rx, uint16_t* idx, uint16_t max_len, EchoMode mode) {
  if (rx == '\r' || rx == '\n') {
    // Handle [ENTER] : send carriage return + newline and exit function
    HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n", 2, 10);
    buffer[*idx] = '\0';
    return 1;

  } else if (rx == '\b' || rx == 0x7f) {
    // Handle [BACKSPACE] : delete character (effectively move back and overwrite the character with a whitespace)
    if (*idx > 0) {
      if (mode != ECHO_SILENT) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"\b \b", 3, 10);
      }
      (*idx)--;
    }

  } else if (*idx < (max_len - 1)) {
    if (rx >= ' ' && rx <= '~') {
      // Accept ASCII-printable character (between ' ' / 0x20 and '~' / 0x7e)
      if (mode == ECHO_NORMAL) {
        HAL_UART_Transmit(&huart1, (uint8_t*)&rx, 1, 10);
      } else if (mode == ECHO_MASKED) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"*", 1, 10);
      }
      buffer[(*idx)++] = rx;
    }
  }
  return 0;
}

void Utils_UART_Readline_Ex(char* buffer, uint16_t max_len, EchoMode mode) {
  uint8_t rx;
  uint16_t idx = 0;

  memset(buffer, 0, max_len);

  while (1) {
    // Receive characters one by one
    if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK) {
      if (Utils_UART_Readchar(buffer, rx, &idx, max_len, mode) == 1) {
        return;
      }
    }
  }
}
