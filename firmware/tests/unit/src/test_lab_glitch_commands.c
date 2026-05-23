#include "../lib/unity.h"
#include "../../../dvh/labs/03_glitch/src/lab_glitch_commands.c"
#include "../../../dvh/labs/03_glitch/src/lab_glitch_data.c"
#include "../../../dvh/shared/src/utils_secrets.c"
#include "../mocks/mock_utils_uart.c"
#include "../mocks/mock_utils_screen.c"
#include <string.h>

Lab_Glitch_State state;

void setUp(void) {
  SPY_Screen_Clear();
  Lab_Glitch_Cmd_InitState(&state);
}

void tearDown(void) {}

void test_Lab_Glitch_Cmd_ProcessByte_should_printui(void) {
  Lab_Glitch_Cmd_ProcessByte(&state, 0x00, 0);

  TEST_ASSERT_TRUE(state.ui_printed);
  TEST_ASSERT_NOT_NULL(strstr(SPY_Screen_LastText, "Running integrity checks"));
}

void test_Lab_Glitch_Cmd_ProcessByte_should_notprintui(void) {
  state.ui_printed = true;
  Lab_Glitch_Cmd_ProcessByte(&state, 0x00, 0);

  TEST_ASSERT_NULL(strstr(SPY_Screen_LastText, "Running integrity checks"));
}

void test_Lab_Glitch_Cmd_ProcessByte_should_printcorrupted(void) {
  Lab_Glitch_Cmd_ProcessByte(&state, 0xc0, 10000);
  TEST_ASSERT_NOT_NULL(strstr(SPY_Screen_LastText, "CORRUPTED"));
}

void test_Lab_Glitch_Cmd_ProcessByte_should_printunstable(void) {
  Lab_Glitch_Cmd_ProcessByte(&state, 0xca, 10000);
  TEST_ASSERT_NOT_NULL(strstr(SPY_Screen_LastText, "UNSTABLE"));
  TEST_ASSERT_TRUE(state.flag_one_printed);
}

void test_Lab_Glitch_Cmd_ProcessByte_should_printverified(void) {
  Lab_Glitch_Cmd_ProcessByte(&state, 0xcb, 1000);
  TEST_ASSERT_NOT_NULL(strstr(SPY_Screen_LastText, "VERIFIED"));
  TEST_ASSERT_TRUE(state.flag_two_printed);
}

void test_Lab_Glitch_Cmd_ProcessByte_should_ratelimit(void) {
  Lab_Glitch_Cmd_ProcessByte(&state, 0xc0, 100);
  TEST_ASSERT_EQUAL(state.last_dot, 0);

  Lab_Glitch_Cmd_ProcessByte(&state, 0xc0, 600);
  TEST_ASSERT_EQUAL(state.last_dot, 600);

  Lab_Glitch_Cmd_ProcessByte(&state, 0xca, 100);
  TEST_ASSERT_EQUAL(state.last_crash, 0);

  Lab_Glitch_Cmd_ProcessByte(&state, 0xca, 1500);
  TEST_ASSERT_EQUAL(state.last_crash, 1500);
}

void test_Lab_Glitch_Cmd_ProcessByte_should_printgarbage(void) {
  state.ui_printed = true;
  Lab_Glitch_Cmd_ProcessByte(&state, 0xbb, 0);
  TEST_ASSERT_EQUAL_HEX8(SPY_Screen_LastText[0], 0xbb);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Lab_Glitch_Cmd_ProcessByte_should_printui);
  RUN_TEST(test_Lab_Glitch_Cmd_ProcessByte_should_notprintui);
  RUN_TEST(test_Lab_Glitch_Cmd_ProcessByte_should_printcorrupted);
  RUN_TEST(test_Lab_Glitch_Cmd_ProcessByte_should_printunstable);
  RUN_TEST(test_Lab_Glitch_Cmd_ProcessByte_should_printverified);
  RUN_TEST(test_Lab_Glitch_Cmd_ProcessByte_should_ratelimit);
  RUN_TEST(test_Lab_Glitch_Cmd_ProcessByte_should_printgarbage);

  return UNITY_END();
}
