#include "../lib/unity.h"
#include "../mocks/mock_st7789.c"
#include "../../../dvh/shared/src/utils_screen.c"

void setUp(void) {}

void tearDown(void) {}

void test_Utils_Screen_Write_should_increment_cursor(void) {
  Utils_Screen_Fill(UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("Line 1", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("Line 2", UTILS_SCREEN_STANDARD);

  TEST_ASSERT_EQUAL(18, SPY_ST7789_Y);
}

void test_Utils_Screen_Write_should_wrap_vertical(void) {
  Utils_Screen_Fill(UTILS_SCREEN_STANDARD);

  // 240 / 18 = 13.3, wrap happens at 14th line
  for (int i = 0; i < 14; i++) {
    Utils_Screen_Write("SPAM", UTILS_SCREEN_STANDARD);
  }

  TEST_ASSERT_EQUAL(0, SPY_ST7789_Y); 
}

void test_Utils_Screen_Write_should_wrap_horizontal(void) {
  Utils_Screen_Fill(UTILS_SCREEN_STANDARD);

  // 43 * 11 = 473, 473 / 240 = 1.97, wraps once
  Utils_Screen_Write("This is a very long string that should wrap", UTILS_SCREEN_STANDARD);
  Utils_Screen_Write("Next Line", UTILS_SCREEN_STANDARD); // Dummy line to force the static cursor into the spy

  // 18 * 2 = 36
  TEST_ASSERT_EQUAL(36, SPY_ST7789_Y);
}

void test_Utils_Screen_Write_should_colorize(void) {
  Utils_Screen_Write("WARNING", UTILS_SCREEN_WARNING);

  TEST_ASSERT_EQUAL(YELLOW, SPY_ST7789_FG);
  TEST_ASSERT_EQUAL(RED, SPY_ST7789_BG);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Utils_Screen_Write_should_increment_cursor);
  RUN_TEST(test_Utils_Screen_Write_should_wrap_vertical);
  RUN_TEST(test_Utils_Screen_Write_should_wrap_horizontal);
  RUN_TEST(test_Utils_Screen_Write_should_colorize);

  return UNITY_END();
}
