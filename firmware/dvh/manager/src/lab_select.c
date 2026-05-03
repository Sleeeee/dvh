#include "lab_select.h"
#include "main.h"

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
