#include "lab_select.h"
#include "main.h"

// WARNING : DVH 0.1 GPIO alias definition
#define LAB_BIT0_Pin GPIO_PIN_4
#define LAB_BIT0_GPIO_Port GPIOA
#define LAB_BIT1_Pin GPIO_PIN_5
#define LAB_BIT1_GPIO_Port GPIOA
#define LAB_BIT2_Pin GPIO_PIN_6
#define LAB_BIT2_GPIO_Port GPIOA
#define LAB_BIT3_Pin GPIO_PIN_7
#define LAB_BIT3_GPIO_Port GPIOA

void LabSelect_Init(void) {
  // WARNING : DVH 0.1 repurposes SPI pins as GPIOs
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = LAB_BIT0_Pin | LAB_BIT1_Pin | LAB_BIT2_Pin | LAB_BIT3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  HAL_GPIO_Init(LAB_BIT0_GPIO_Port, &GPIO_InitStruct);
}

void LabSelect_Blink(uint8_t count) {
  for (int i = 0; i < count; i++) {
    HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_SET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
  }
}

uint8_t LabSelect_ComputeID(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
  return b0 | (b1 << 1) | (b2 << 2) | (b3 << 3);
}

uint8_t LabSelect_Read(void) {
  uint8_t b0 = HAL_GPIO_ReadPin(LAB_BIT0_GPIO_Port, LAB_BIT0_Pin) == GPIO_PIN_SET;
  uint8_t b1 = HAL_GPIO_ReadPin(LAB_BIT1_GPIO_Port, LAB_BIT1_Pin) == GPIO_PIN_SET;
  uint8_t b2 = HAL_GPIO_ReadPin(LAB_BIT2_GPIO_Port, LAB_BIT2_Pin) == GPIO_PIN_SET;
  uint8_t b3 = HAL_GPIO_ReadPin(LAB_BIT3_GPIO_Port, LAB_BIT3_Pin) == GPIO_PIN_SET;

  return LabSelect_ComputeID(b0, b1, b2, b3);
}
