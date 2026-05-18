#ifndef UTILS_SCREEN_H
#define UTILS_SCREEN_H

typedef enum {
  UTILS_SCREEN_OK,
  UTILS_SCREEN_ERROR
} Utils_Screen_StatusTypeDef;

Utils_Screen_StatusTypeDef Utils_Screen_Welcome(void);

Utils_Screen_StatusTypeDef Utils_Screen_Display_Lab(int lab_id);

Utils_Screen_StatusTypeDef Utils_Screen_Lab_Not_Found();

Utils_Screen_StatusTypeDef Utils_Screen_Lab_Init_Failed();

#endif
