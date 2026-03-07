#ifndef LAB_SELECT_H
#define LAB_SELECT_H

#include <stdint.h>
#include <stdbool.h>

void LabSelect_Init(void);

uint8_t LabSelect_Read(void);

bool LabSelect_Reset_Pressed(void);

#endif
