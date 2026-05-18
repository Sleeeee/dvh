#include "lab_bootstrap.h"
#include "lab_select.h"
#include "lab_blink.h"
#include "lab_registry.h"
#include "ilab.h"
#include "utils_screen.h"
#include "main.h"

void LabBootstrap_Start(void) {
  Utils_Screen_Welcome();

  uint8_t lab_id = LabSelect_Read();
  Utils_Screen_Display_Lab(lab_id);
  LabBlink_Selected(lab_id);
  ILab* lab = LabRegistry_GetById(lab_id);

  if (lab == NULL) {
    // Unable to load a lab
    while (1) {
      Utils_Screen_Lab_Not_Found(); // This creates a blink effect because the screen has to re-render every time it enters the loop
      LabBlink_Broken();
    }
  }

  if (LabSelect_Reset_Pressed()) {
    LabBlink_Reset();
    if (lab->reset() != LAB_OK) {
      while (1) { LabBlink_Broken(); } // Lab reset failed
    }
    LabBlink_Continue();
  }

  if (lab->init() != LAB_OK) {
    // Lab init failed
    while (1) {
      Utils_Screen_Lab_Init_Failed();
      LabBlink_Broken();
    }
  }

  while (1) {
    lab->loop();
  }
}
