#include <avr/io.h>
#include <util/delay.h>

#define TX_PIN PB3
#define TRIG_PIN PB4
#define BIT_DELAY 715

void HAL_Init(void) {
  // Slow down the clock using prescaler (125 kHz)
  CLKPR = (1 << CLKPCE);
  CLKPR = (1 << CLKPS2) | (1 << CLKPS1);

  DDRB |= (1 << TX_PIN) | (1 << TRIG_PIN);
}

uint8_t HAL_GetResetCause(void) {
  return MCUSR;
}

void HAL_ClearResetCause(void) {
  MCUSR = 0;
}

uint8_t HAL_IsExternalReset(uint8_t reset_cause) {
  return reset_cause & (1 << EXTRF);
}

void HAL_TX_High() {
  PORTB |= (1 << TX_PIN);
}

void HAL_TRIG_High() {
  PORTB |= (1 << TRIG_PIN);
}

void HAL_TRIG_Low() {
  PORTB &= ~(1 << TRIG_PIN);
}

void HAL_UART_TransmitByte(uint8_t data) {
  PORTB &= ~(1 << TX_PIN); // Start bit (LOW)
  _delay_us(BIT_DELAY);

  // Data bits (8 bits, LSB first)
  for (uint8_t i = 0; i < 8; i++) {
    if (data & 0x01) {
      PORTB |= (1 << TX_PIN); // HIGH
    } else {
      PORTB &= ~(1 << TX_PIN); // LOW
    }
    _delay_us(BIT_DELAY);
    data >>= 1;
  }

  PORTB |= (1 << TX_PIN); // Stop bit (HIGH)
  _delay_us(BIT_DELAY);
}
