#include "mock_st7789.c"
#include "../../../dvh/shared/include/utils_screen.h"
#include <string.h>

uint8_t SPY_Screen_AccessDenied_Calls = 0;
uint8_t SPY_Screen_Anonymous_Calls = 0;
uint8_t SPY_Screen_User_Calls = 0;
uint8_t SPY_Screen_Root_Calls = 0;

char SPY_Screen_LastText[128] = {0};
Utils_Screen_State SPY_Screen_LastState = 0;
int SPY_Screen_WriteChar_CallCount = 0;
int SPY_Screen_Write_CallCount = 0;
int SPY_Screen_FillWriteChar_CallCount = 0;
int SPY_Screen_FillWrite_CallCount = 0;

void SPY_Screen_Clear(void) {
  SPY_Screen_AccessDenied_Calls = 0;
  SPY_Screen_Anonymous_Calls = 0;
  SPY_Screen_User_Calls = 0;
  SPY_Screen_Root_Calls = 0;
  SPY_Screen_Write_CallCount = 0;
  SPY_Screen_FillWrite_CallCount = 0;
  SPY_Screen_LastState = 0;
  memset(SPY_Screen_LastText, 0, sizeof(SPY_Screen_LastText));
}

void Utils_Screen_Access_Denied(void) { SPY_Screen_AccessDenied_Calls++; }
void Utils_Screen_UART_Anonymous(void) { SPY_Screen_Anonymous_Calls++; }
void Utils_Screen_UART_User(void) { SPY_Screen_User_Calls++; }
void Utils_Screen_UART_Root(void) { SPY_Screen_Root_Calls++; }

void Utils_Screen_WriteChar(char c, Utils_Screen_State state) {
  SPY_Screen_WriteChar_CallCount++;
  SPY_Screen_LastState = state;
  int len = strlen(SPY_Screen_LastText);
  if (len < sizeof(SPY_Screen_LastText) - 1) {
      SPY_Screen_LastText[len] = c;
      SPY_Screen_LastText[len+1] = '\0';
  }
}

void Utils_Screen_Write(char* text, Utils_Screen_State state) {
  SPY_Screen_Write_CallCount++;
  SPY_Screen_LastState = state;
  strncat(SPY_Screen_LastText, text, sizeof(SPY_Screen_LastText) - strlen(SPY_Screen_LastText) - 1);
}

void Utils_Screen_Fill_WriteChar(char c, Utils_Screen_State state) {
  SPY_Screen_FillWriteChar_CallCount++;
  SPY_Screen_LastState = state;
  memset(SPY_Screen_LastText, 0, sizeof(SPY_Screen_LastText)); // Clear screen
  SPY_Screen_LastText[0] = c;
}

void Utils_Screen_Fill_Write(char* text, Utils_Screen_State state) {
  SPY_Screen_FillWrite_CallCount++;
  SPY_Screen_LastState = state;
  memset(SPY_Screen_LastText, 0, sizeof(SPY_Screen_LastText)); // Clear screen
  strncpy(SPY_Screen_LastText, text, sizeof(SPY_Screen_LastText) - 1);
}
