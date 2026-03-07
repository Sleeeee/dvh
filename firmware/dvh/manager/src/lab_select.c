#include "lab_select.h"
#include "main.h"

// WARNING : DVH 0.1 GPIO alias definition
#define LAB_RST_Pin GPIO_PIN_3
#define LAB_RST_GPIO_Port GPIOA
#define LAB_S0_Pin GPIO_PIN_4
#define LAB_S0_GPIO_Port GPIOA
#define LAB_S1_Pin GPIO_PIN_5
#define LAB_S1_GPIO_Port GPIOA
#define LAB_S2_Pin GPIO_PIN_6
#define LAB_S2_GPIO_Port GPIOA
#define LAB_S3_Pin GPIO_PIN_7
#define LAB_S3_GPIO_Port GPIOA

void LabSelect_Init(void) {
  // WARNING : DVH 0.1 repurposes SPI pins as GPIOs
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = LAB_RST_Pin | LAB_S0_Pin | LAB_S1_Pin | LAB_S2_Pin | LAB_S3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  HAL_GPIO_Init(LAB_RST_GPIO_Port, &GPIO_InitStruct);
}

uint8_t LabSelect_ComputeID(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3) {
  return s0 | (s1 << 1) | (s2 << 2) | (s3 << 3);
}

uint8_t LabSelect_Read(void) {
  uint8_t s0 = HAL_GPIO_ReadPin(LAB_S0_GPIO_Port, LAB_S0_Pin) == GPIO_PIN_SET;
  uint8_t s1 = HAL_GPIO_ReadPin(LAB_S1_GPIO_Port, LAB_S1_Pin) == GPIO_PIN_SET;
  uint8_t s2 = HAL_GPIO_ReadPin(LAB_S2_GPIO_Port, LAB_S2_Pin) == GPIO_PIN_SET;
  uint8_t s3 = HAL_GPIO_ReadPin(LAB_S3_GPIO_Port, LAB_S3_Pin) == GPIO_PIN_SET;

  return LabSelect_ComputeID(s0, s1, s2, s3);
}

bool LabSelect_Reset_Pressed(void) {
  return HAL_GPIO_ReadPin(LAB_RST_GPIO_Port, LAB_RST_Pin) == GPIO_PIN_SET;
}
