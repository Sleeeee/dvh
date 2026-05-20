#include "utils_isp.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;

uint8_t Utils_ISP_Transmit(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t ret_idx) {
  uint8_t tx_data[4] = {b1, b2, b3, b4};
  uint8_t rx_data[4] = {0};
  HAL_SPI_TransmitReceive(&hspi2, tx_data, rx_data, 4, HAL_MAX_DELAY);
  return rx_data[ret_idx];
}

Utils_ISP_StatusTypeDef Utils_ISP_ProgrammingEnable(void) {
  uint8_t ret = Utils_ISP_Transmit(0xac, 0x53, 0x00, 0x00, 2);
  if (ret == 0x53) return UTILS_ISP_OK;
  return UTILS_ISP_ERROR;
}

Utils_ISP_StatusTypeDef Utils_ISP_ProgrammingEnable_Retry(uint8_t count) {
  uint8_t retries = 0;

  while (retries < count) {
    // Pulse RESET
    HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(25);

    if (Utils_ISP_ProgrammingEnable() == UTILS_ISP_OK) return UTILS_ISP_OK;
    retries++;
  }

  return false;
}

void Utils_ISP_ReadDeviceCode(uint8_t* device_code, size_t size) {
  if ((device_code == NULL) || (size < 3)) return;

  device_code[0] = Utils_ISP_Transmit(0x30, 0x00, 0x00, 0x00, 3); // vendor
  device_code[1] = Utils_ISP_Transmit(0x30, 0x00, 0x01, 0x00, 3); // family and flash size
  device_code[2] = Utils_ISP_Transmit(0x30, 0x00, 0x02, 0x00, 3); // part ID
}

Utils_ISP_StatusTypeDef Utils_ISP_ChipErase(void) {
  uint8_t ret = Utils_ISP_Transmit(0xac, 0x80, 0x00, 0x00, 2);
  if (ret == 0x80) return UTILS_ISP_OK;
  return UTILS_ISP_ERROR;
}

void Utils_ISP_Write(unsigned char* data, unsigned int len) {
  uint32_t b_addr = 0; // byte address

  while (b_addr < len) {
    uint8_t b_low = data[b_addr];
    uint8_t b_high = 0xff; // Default pad if odd payload
    if ((b_addr + 1) < len) {
      b_high = data[b_addr + 1];
    }

    uint16_t w_addr = b_addr / 2; // word address
    uint8_t w_lsb = w_addr & 0xff;
    uint8_t w_msb = (w_addr >> 8) & 0xff;

    // Load Program Memory Page
    Utils_ISP_Transmit(0x40, w_msb, w_lsb, b_low, 0);
    Utils_ISP_Transmit(0x48, w_msb, w_lsb, b_high, 0);

    b_addr += 2;

    // Only write when end of page or end of payload
    if (((b_addr % 64) == 0) || (b_addr >= len)) {
      uint16_t p_addr = (w_addr / 32) * 32; // page base address
      uint8_t p_lsb = p_addr & 0xff;
      uint8_t p_msb = (p_addr >> 8) & 0xff;

      // Write Program Memory Page
      Utils_ISP_Transmit(0x4c, p_msb, p_lsb, 0x00, 0);
      HAL_Delay(5);
    }
  }
}
