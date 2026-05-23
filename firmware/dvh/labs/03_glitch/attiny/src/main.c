#include "attiny_hal.h"
#include <util/delay.h>

#define CANARY_SIZE 16
uint8_t canary[CANARY_SIZE] __attribute__ ((section (".noinit")));

int main(void) {
  uint8_t reset_cause = HAL_GetResetCause();
  HAL_ClearResetCause();

  HAL_Init();
  HAL_TX_High();
  HAL_TRIG_Low();
  _delay_ms(100);

  uint8_t is_glitch = 1;
  // Inspect canary for expected bytes
  for (uint8_t i = 0; i < CANARY_SIZE; i++) {
    uint8_t expected_val = (i % 2 == 0) ? 0xAA : 0x55;
    if (canary[i] != expected_val) {
      is_glitch = 0;
      break;
    }
  }

  // Refresh canary for next time
  if (!is_glitch) {
    for (uint8_t i = 0; i < CANARY_SIZE; i++) {
      canary[i] = (i % 2 == 0) ? 0xAA : 0x55;
    }
  }

  if (is_glitch && !HAL_IsExternalReset(reset_cause)) {
    HAL_UART_TransmitByte(0xca); // First flag / Crashed
  }

  while(1) {
    HAL_TRIG_High();
    _delay_us(20);

    volatile uint16_t count = 0;
    for (volatile uint16_t i = 0; i < 500; i++) {
        count++;
    }

    HAL_TRIG_Low();

    if (count != 500) {
      _delay_ms(10);
      HAL_UART_TransmitByte(0xcb); // Second flag
      _delay_ms(10000);
    } else {
      HAL_UART_TransmitByte(0xc0); // Failed
    }

    _delay_ms(2);
  }
}
