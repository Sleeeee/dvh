#include "../lib/unity.h"
#include "../../../select/lab_select.c"
#include <stdint.h>

void setUp(void) {}

void tearDown(void) {}

void test_LabSelect_ComputeID_should_compute0(void) {
  uint8_t result = LabSelect_ComputeID(0, 0, 0, 0);
  TEST_ASSERT_EQUAL_UINT8(0, result);
}

void test_LabSelect_ComputeID_should_compute6(void) {
  uint8_t result = LabSelect_ComputeID(0, 1, 1, 0);
  TEST_ASSERT_EQUAL_UINT8(6, result);
}

void test_LabSelect_ComputeID_should_compute13(void) {
  uint8_t result = LabSelect_ComputeID(1, 0, 1, 1);
  TEST_ASSERT_EQUAL_UINT8(13, result);
};

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_LabSelect_ComputeID_should_compute0);
  RUN_TEST(test_LabSelect_ComputeID_should_compute6);
  RUN_TEST(test_LabSelect_ComputeID_should_compute13);

  return UNITY_END();
}
