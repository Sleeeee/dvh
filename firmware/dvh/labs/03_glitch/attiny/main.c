#include <avr/io.h>
#include <util/delay.h>

#define TX_PIN PB3 
#define TRIG_PIN PB4
#define BIT_DELAY 833 // 1200 baud (1,000,000 / 1200)

void uart_tx_bit(uint8_t bit) {
  if (bit) {
    PORTB |= (1 << TX_PIN);  // HIGH
  } else {
    PORTB &= ~(1 << TX_PIN); // LOW
  }
  _delay_us(BIT_DELAY);
}

void uart_tx_byte(uint8_t data) {
  uart_tx_bit(0); // Start bit

  // Data (8 bits, LSB first)
  for (uint8_t i = 0; i < 8; i++) {
    uart_tx_bit(data & 0x01);
    data >>= 1;
  }

  uart_tx_bit(1); // Stop bit
  _delay_us(BIT_DELAY); 
}

void uart_print(const char* str) {
  while (*str) {
    uart_tx_byte(*str++);
  }
}

int main(void) {
  DDRB |= (1 << TX_PIN); // Configure output
  PORTB |= (1 << TX_PIN); // UART idle high
  PORTB &= ~(1 << TRIG_PIN); // TRIG idle low
  _delay_ms(100); 

  while(1) {
    volatile uint8_t never = 0;

    uart_print("[UART] Verifying ATTiny signature");
    _delay_ms(500);
    uart_tx_byte('.');
    _delay_ms(500);
    uart_tx_byte('.');
    _delay_ms(500);
    uart_tx_byte('.');
    _delay_us(50); // Very small delay to facilitate glitch

    // PORTB |= (1 << TRIG_PIN); // Raise TRIG

    if (never) {
      // PORTB &= ~(1 << TRIG_PIN); // Lower TRIG
      uart_print("GRANTED");
      _delay_ms(10000);
    } else {
      // PORTB &= ~(1 << TRIG_PIN); // Lower TRIG
      uart_print("DENIED ");
    }

    _delay_ms(4000);
    uart_tx_byte('!'); // Clear screen
    _delay_ms(1000);
  }
}
