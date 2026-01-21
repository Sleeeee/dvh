#ifndef STM32F1xx_HAL_H
#define STM32F1xx_HAL_H

#include <stdint.h>

// Create the type for the pointer, the content does not matter
typedef struct {
  uint32_t dummy;
} GPIO_TypeDef;

static inline void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
  (void)GPIOx;
  (void)GPIO_Pin;
}

static inline void HAL_Delay(uint32_t Delay) {
  (void)Delay;
}

#endif
