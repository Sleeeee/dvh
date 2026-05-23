#include "fonts.h"
#include <string.h>

FontDef Font_11x18 = {11, 18, NULL};
uint16_t SPY_ST7789_X = 0;
uint16_t SPY_ST7789_Y = 0;
uint16_t SPY_ST7789_FG = 0;
uint16_t SPY_ST7789_BG = 0;
uint16_t SPY_ST7789_Init_CallCount = 0;
char SPY_ST7789_Buffer[128] = {0};

void ST7789_Init(void) {
  SPY_ST7789_Init_CallCount++;
}

void ST7789_Fill_Color(uint16_t color) {
  SPY_ST7789_BG = color;
}

void ST7789_WriteChar(uint16_t x, uint16_t y, const char c, FontDef font, uint16_t color, uint16_t bgcolor) {
  (void)font;
  SPY_ST7789_FG = color;
  SPY_ST7789_BG = bgcolor;
  SPY_ST7789_X = x;
  SPY_ST7789_Y = y;
  strncpy(SPY_ST7789_Buffer, &c, 1);
}

void ST7789_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor) {
  (void)font;
  SPY_ST7789_FG = color;
  SPY_ST7789_BG = bgcolor;
  SPY_ST7789_X = x;
  SPY_ST7789_Y = y;
  strncpy(SPY_ST7789_Buffer, str, sizeof(SPY_ST7789_Buffer) - 1);
}
