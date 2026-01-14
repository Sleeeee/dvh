#include "../lib/unity.h"
#include "../../../labs/00_swd/lab_swd.c"
#include <string.h>

volatile char buffer[100];

void setUp(void) {
  memset((char*)buffer, 0, 100);
}

void tearDown(void) {}

void test_Solve_Flag_One_should_copy(void) {
  const char mock_flag_one[] = "DVH{m0ck_fl4g_0n3_ffffffffff}";
  Solve_Flag_One(mock_flag_one, buffer);

  TEST_ASSERT_EQUAL_STRING(mock_flag_one, (char*)buffer);
}

void test_Solve_Flag_Two_should_decrypt(void) {
  const unsigned char mock_flag_two_encrypted[] = {0xef, 0xfd, 0xe3, 0xd0, 0xc6, 0x9b, 0xc8, 0xc0, 0xf4, 0xcd, 0xc7, 0x9f, 0xcc, 0xf4, 0xdf, 0xdc, 0x9b, 0xf4, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xd6};
  const char mock_flag_two_plain[] = "DVH{m0ck_fl4g_tw0_ffffffffff}";
  Solve_Flag_Two(mock_flag_two_encrypted, sizeof(mock_flag_two_encrypted), buffer);

  TEST_ASSERT_EQUAL_STRING(mock_flag_two_plain, (char*)buffer);
}

void test_Solve_Flag_Three_should_decrypt(void) {
  const unsigned char mock_flag_three_encrypted[] = {0xe8, 0xfa, 0xec, 0x1f, 0xf1, 0xd4, 0x07, 0x0f, 0x03, 0x0a, 0xf0, 0xd8, 0x0b, 0x03, 0x18, 0x0c, 0x16, 0xd7, 0xd7, 0x03, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x01};
  const char mock_flag_three_plain[] = "DVH{m0ck_fl4g_thr33_ffffffffff}";
  Solve_Flag_Three(mock_flag_three_encrypted, sizeof(mock_flag_three_encrypted), buffer);

  TEST_ASSERT_EQUAL_STRING(mock_flag_three_plain, (char*)buffer);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Solve_Flag_One_should_copy);
  RUN_TEST(test_Solve_Flag_Two_should_decrypt);
  RUN_TEST(test_Solve_Flag_Three_should_decrypt);

  return UNITY_END();
}
