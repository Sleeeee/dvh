#include "utils_screen.h"
#include "main.h"
#include "st7789.h"
#include <stdio.h>

Utils_Screen_StatusTypeDef Utils_Screen_Welcome(void) {
  HAL_GPIO_WritePin(SCREEN_BLK_GPIO_Port, SCREEN_BLK_Pin, GPIO_PIN_SET);
  ST7789_Init();
  ST7789_Fill_Color(BLACK);
  ST7789_WriteString(10, 10, "DVH v1.0.0", Font_11x18, RED, BLACK);

  return UTILS_SCREEN_OK;
}

Utils_Screen_StatusTypeDef Utils_Screen_Display_Lab(int lab_id) {
  char text[32];
  snprintf(text, sizeof(text), "Loading lab %d...", lab_id);

  ST7789_Fill_Color(BLACK);
  ST7789_WriteString(10, 10, text, Font_11x18, RED, BLACK);

  return UTILS_SCREEN_OK;
}

Utils_Screen_StatusTypeDef Utils_Screen_Lab_Not_Found(void) {
  ST7789_Fill_Color(BLACK);
  ST7789_WriteString(10, 10, "Requested lab could not be found !", Font_11x18, RED, BLACK);

  return UTILS_SCREEN_OK;
}

Utils_Screen_StatusTypeDef Utils_Screen_Lab_Init_Failed(void) {
  ST7789_Fill_Color(BLACK);
  ST7789_WriteString(10, 10, "Failed lab initialization.", Font_11x18, RED, BLACK);

  return UTILS_SCREEN_OK;
}
