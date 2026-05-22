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

void uart_print(const char* str) {
  while (*str) {
    uart_tx_byte(*str++);
  }
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
    uart_tx_byte(0xca); // Magic byte
  }

  while(1) {
    uart_print("[UART] Verifying ATTiny signature");
    _delay_ms(500);
    uart_tx_byte('.');
    _delay_ms(500);
    uart_tx_byte('.');
    _delay_ms(500);
    uart_tx_byte('.');
    _delay_ms(20); 

    PORTB |= (1 << TRIG_PIN);
    _delay_us(200);

    // If any loop iteration gets skipped, count will be wrong
    volatile uint16_t count = 0;
    for (volatile uint16_t i = 0; i < 500; i++) {
        count++;
    }

    PORTB &= ~(1 << TRIG_PIN); // Lower TRIG

    if (count != 500) {
      uart_tx_byte(0xcb);
      _delay_ms(10000);
    } else {
      uart_print("DENIED ");
    }

    _delay_ms(4000);
    uart_tx_byte('!'); // Clear screen
    _delay_ms(1000);
  }
}
