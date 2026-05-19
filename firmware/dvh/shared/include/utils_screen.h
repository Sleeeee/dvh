#ifndef UTILS_SCREEN_H
#define UTILS_SCREEN_H

typedef enum {
  UTILS_SCREEN_STANDARD = 0,
  UTILS_SCREEN_SUCCESS,
  UTILS_SCREEN_WARNING,
} Utils_Screen_State;

void Utils_Screen_Fill(Utils_Screen_State status);

void Utils_Screen_Write(char* text, Utils_Screen_State status);

void Utils_Screen_Fill_Write(char* text, Utils_Screen_State status);

void Utils_Screen_Welcome(void);

void Utils_Screen_Display_Lab(int lab_id);

void Utils_Screen_Lab_Not_Found(void);

void Utils_Screen_Lab_Reset(void);

void Utils_Screen_Lab_Reset_Failed(void);

void Utils_Screen_Lab_Init_Failed(void);

void Utils_Screen_Lab_Loaded(void);

#endif
