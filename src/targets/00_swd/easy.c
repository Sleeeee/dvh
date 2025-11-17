#include <stdio.h>
#include "pico/stdlib.h"

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 1000
#endif

__attribute__((used)) const char flag[] = "DVH{tH15_w45_34sy_3n0uGh}";

void led_init(void) {
#ifdef PICO_DEFAULT_LED_PIN
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

void led_set(bool on) {
#ifdef PICO_DEFAULT_LED_PIN
  gpio_put(PICO_DEFAULT_LED_PIN, on);
#endif
}

int main() {
  led_init();

  // Force a read of the flag to prevent it from being discarded
  printf("%s\n", flag);

  while (true) {
    led_set(true);
    sleep_ms(LED_DELAY_MS);
    led_set(false);
    sleep_ms(LED_DELAY_MS);
  }
}
