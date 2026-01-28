#ifndef STM32F1xx_HAL_H
#define STM32F1xx_HAL_H

#include <stdint.h>

// General HAL mocks
#define GPIO_PIN_SET   1
#define GPIO_PIN_RESET 0
typedef uint8_t GPIO_PinState;

#define GPIO_PIN_0  0x0000
#define GPIO_PIN_1  0x0000
#define GPIO_PIN_2  0x0000
#define GPIO_PIN_3  0x0000
#define GPIO_PIN_4  0x0000
#define GPIO_PIN_5  0x0000
#define GPIO_PIN_6  0x0000
#define GPIO_PIN_7  0x0000

#define GPIO_MODE_INPUT 0x00
#define GPIO_PULLDOWN   0x00

typedef struct {
  uint32_t Pin;
  uint32_t Mode;
  uint32_t Pull;
} GPIO_InitTypeDef;

// Create the type for its pointer, the content does not matter
typedef struct {
  uint32_t dummy;
} GPIO_TypeDef;

#define __HAL_RCC_GPIOA_CLK_ENABLE() do { } while(0)

static inline void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
  (void)GPIOx;
  (void)GPIO_Init;
}

static inline GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
  (void)GPIOx;
  (void)GPIO_Pin;
  return GPIO_PIN_RESET;
}

static inline void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
  (void)GPIOx;
  (void)GPIO_Pin;
}

static inline void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
  (void)GPIOx;
  (void)GPIO_Pin;
}

static inline void HAL_Delay(uint32_t Delay) {
  (void)Delay;
}

// UART mocks
typedef struct {
  uint32_t dummy;
} UART_HandleTypeDef;

typedef enum {
  HAL_OK = 0x00U,
  HAL_ERROR = 0x01U
} HAL_StatusTypeDef;

HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

extern char SPY_UART_Buffer[128];
extern int SPY_UART_CallCount;

void SPY_UART_Clear(void);

#endif
