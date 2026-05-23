#ifndef ATTINY_HAL_H
#define ATTINY_HAL_H

#include <stdint.h>

void HAL_Init(void);

uint8_t HAL_GetResetCause(void);

void HAL_ClearResetCause(void);

uint8_t HAL_IsExternalReset(uint8_t reset_cause);

void HAL_TX_High(void);

void HAL_TRIG_High(void);

void HAL_TRIG_Low(void);

void HAL_UART_TransmitByte(uint8_t data);

#endif
