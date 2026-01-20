#import "lab_select.h"
#import "main.h"

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

uint8_t LabSelect_Read(void) {
  uint8_t lab_id = 0;

  if (HAL_GPIO_ReadPin(LAB_BIT0_GPIO_Port, LAB_BIT0_Pin) == GPIO_PIN_SET) lab_id |= (1 << 0);
  if (HAL_GPIO_ReadPin(LAB_BIT1_GPIO_Port, LAB_BIT1_Pin) == GPIO_PIN_SET) lab_id |= (1 << 1);
  if (HAL_GPIO_ReadPin(LAB_BIT2_GPIO_Port, LAB_BIT2_Pin) == GPIO_PIN_SET) lab_id |= (1 << 2);
  if (HAL_GPIO_ReadPin(LAB_BIT3_GPIO_Port, LAB_BIT3_Pin) == GPIO_PIN_SET) lab_id |= (1 << 3);

  return lab_id;
}
