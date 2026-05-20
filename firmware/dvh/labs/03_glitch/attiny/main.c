#include <avr/io.h>
#include <util/delay.h>

#define TX_PIN PB3 
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
  PORTB |= (1 << TX_PIN); // UART idle state
  _delay_ms(100); 

  while(1) {
      uart_print("Hello World!");
      _delay_ms(2000); 
  }
}
