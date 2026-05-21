#include "utils_screen.h"
#include "constants.h"
#include "main.h"
#include "st7789.h"
#include "fonts.h"
#include <stdio.h>
#include <string.h>

#define BASE_X_OFFSET 0
#define BASE_Y_OFFSET 0

static uint16_t cursor_x = BASE_X_OFFSET;
static uint16_t cursor_y = BASE_Y_OFFSET;
static FontDef *font = &Font_11x18;

typedef struct {
  uint16_t fg;
  uint16_t bg;
} Utils_Screen_ColorTheme;

static const Utils_Screen_ColorTheme COLOR_THEMES[] = {
  [UTILS_SCREEN_STANDARD] = {WHITE, BLUE},
  [UTILS_SCREEN_HIGHLIGHT] = {RED, BLUE},
  [UTILS_SCREEN_WARNING] = {YELLOW, RED}
};

void Utils_Screen_Fill(Utils_Screen_State state) {
  cursor_x = BASE_X_OFFSET;
  cursor_y = BASE_Y_OFFSET;
  ST7789_Fill_Color(COLOR_THEMES[state].bg);
}

void Utils_Screen_WriteChar(char c, Utils_Screen_State state) {
  ST7789_WriteChar(cursor_x, cursor_y, c, *font, COLOR_THEMES[state].fg, COLOR_THEMES[state].bg);

  cursor_x += font->width;
  if (cursor_x >= (ST7789_WIDTH - font->width)) {
    cursor_y += font->height;
    cursor_x = BASE_X_OFFSET;
  }
  if (cursor_y >= (ST7789_HEIGHT - font->height)) {
    cursor_y = BASE_Y_OFFSET;
  }
}

void Utils_Screen_Write(char* text, Utils_Screen_State state) {
  ST7789_WriteString(cursor_x, cursor_y, text, *font, COLOR_THEMES[state].fg, COLOR_THEMES[state].bg);

  // Move cursor down the correct number of line (1 if no overflow)
  uint16_t requested_width = strlen(text) * font->width;
  uint16_t requested_lines = ((cursor_x + requested_width) / ST7789_WIDTH) + 1;
  cursor_y += (requested_lines * font->height);

  // Vertical bounds check
  if (cursor_y >= (ST7789_HEIGHT - font->height)) {
    cursor_y = BASE_Y_OFFSET;
  }
}

void Utils_Screen_Fill_Write(char* text, Utils_Screen_State state) {
  Utils_Screen_Fill(state);
  Utils_Screen_Write(text, state);
}

void Utils_Screen_Welcome(void) {
  HAL_GPIO_WritePin(SCREEN_BLK_GPIO_Port, SCREEN_BLK_Pin, GPIO_PIN_SET);
  HAL_Delay(100); // Avoid race conditions to ensure the screen can be initialized
  ST7789_Init();

  char text[16];
  snprintf(text, sizeof(text), "[DVH] %s", DVH_VERSION);
  Utils_Screen_Fill_Write(text, UTILS_SCREEN_STANDARD);
}

void Utils_Screen_Display_Lab(int lab_id) {
  char text[32];
  snprintf(text, sizeof(text), "[BOOT] Loading lab %d...", lab_id);
  Utils_Screen_Write(text, UTILS_SCREEN_STANDARD);
}

void Utils_Screen_Lab_Not_Found(void) {
  Utils_Screen_Write("[CRITICAL] Requested lab could not be found !", UTILS_SCREEN_WARNING);
}

void Utils_Screen_Lab_Reset(void) {
  Utils_Screen_Write("[BOOT] Performing lab reset...", UTILS_SCREEN_STANDARD);
}

void Utils_Screen_Lab_Reset_Failed(void) {
  Utils_Screen_Write("[CRITICAL] Failed lab reset.", UTILS_SCREEN_WARNING);
}

void Utils_Screen_Lab_Init_Failed(void) {
  Utils_Screen_Write("[CRITICAL] Failed lab initialization.", UTILS_SCREEN_WARNING);
}

void Utils_Screen_Lab_Loaded(void) {
  Utils_Screen_Write("[SUCCESS] Lab loaded.", UTILS_SCREEN_STANDARD);
}

void Utils_Screen_UART_Anonymous(void) {
  Utils_Screen_Fill_Write("[UART] Unknown user connected !", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("Privilege level :", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("anonymous", UTILS_SCREEN_HIGHLIGHT);
}

void Utils_Screen_UART_User(void) {
  Utils_Screen_Fill_Write("[UART] Authentication successful !", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("Privilege level :", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("user", UTILS_SCREEN_HIGHLIGHT);
}

void Utils_Screen_UART_Root(void) {
  Utils_Screen_Fill_Write("[UART] Access granted !", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("Privilege level :", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("root", UTILS_SCREEN_HIGHLIGHT);
}

void Utils_Screen_Access_Denied(void) {
  for (uint8_t i = 0; i < 3; i++) {
    Utils_Screen_Fill_Write("[WARNING] Access denied !", UTILS_SCREEN_WARNING);
    HAL_Delay(200);
  }
}
