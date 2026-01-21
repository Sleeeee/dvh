#ifndef LAB_SELECT_H
#define LAB_SELECT_H

#include <stdint.h>

void LabSelect_Init(void);

void LabSelect_Blink(uint8_t count);

uint8_t LabSelect_Read(void);

#endif
