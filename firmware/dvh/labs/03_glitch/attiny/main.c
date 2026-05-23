#include <avr/io.h>
#include <util/delay.h>

#define TX_PIN PB3
#define TRIG_PIN PB4
#define BIT_DELAY 715

#define CANARY_SIZE 16
uint8_t canary[CANARY_SIZE] __attribute__ ((section (".noinit")));

void uart_tx_byte(uint8_t data) {
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

int main(void) {
  uint8_t reset_cause = MCUSR;
  MCUSR = 0;

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

  // Slow down the clock using prescaler (125 kHz)
  CLKPR = (1 << CLKPCE);
  CLKPR = (1 << CLKPS2) | (1 << CLKPS1);

  DDRB |= (1 << TX_PIN) | (1 << TRIG_PIN);
  PORTB |= (1 << TX_PIN);
  PORTB &= ~(1 << TRIG_PIN);
  _delay_ms(100);

  if (is_glitch && !(reset_cause & (1 << EXTRF))) {
    uart_tx_byte(0xca); // First flag / Crashed
  }

  while(1) {
    PORTB |= (1 << TRIG_PIN);
    _delay_us(20);

    volatile uint16_t count = 0;
    for (volatile uint16_t i = 0; i < 500; i++) {
        count++;
    }

    PORTB &= ~(1 << TRIG_PIN);

    if (count != 500) {
      _delay_ms(10);
      uart_tx_byte(0xcb); // Second flag
      _delay_ms(10000);
    } else {
      uart_tx_byte(0xc0); // Failed
    }

    _delay_ms(2);
  }
}
