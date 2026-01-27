#include "../lib/unity.h"
#include "../../../shared/src/utils_secrets.c"
#include <string.h>
#include <stdbool.h>

void setUp(void) {}

void tearDown(void) {}

void test_Utils_Secrets_Decrypt_should_decrypt(void) {
  char buffer[64];
  const unsigned char mock_secret_encrypted[] = {0x2e, 0x23, 0x3b, 0x0f, 0x04, 0x5e, 0x09, 0x1e, 0x2c, 0x07, 0x5a, 0x0d, 0x18, 0x46, 0x07, 0x2b, 0x0f, 0x08, 0x0c, 0x13, 0x15, 0x12, 0x0f, 0x08, 0x0c, 0x13, 0x0e};
  const char mock_secret_plain[] = "DVH{m0ck_s3cr3t_ffffffffff}";

  Utils_Secrets_Decrypt(mock_secret_encrypted, sizeof(mock_secret_encrypted), buffer, sizeof(buffer));

  TEST_ASSERT_EQUAL_STRING(mock_secret_plain, buffer);
}

void test_Utils_Secrets_Decrypt_should_not_overflow(void) {
  char small_buffer[8];
  const unsigned char mock_secret_encrypted[] = {0x2e, 0x23, 0x3b, 0x0f, 0x04, 0x5e, 0x09, 0x1e, 0x2c, 0x07, 0x5a, 0x0d, 0x18, 0x46, 0x07, 0x2b, 0x0f, 0x08, 0x0c, 0x13, 0x15, 0x12, 0x0f, 0x08, 0x0c, 0x13, 0x0e};

  Utils_Secrets_Decrypt(mock_secret_encrypted, sizeof(mock_secret_encrypted), small_buffer, sizeof(small_buffer));

  TEST_ASSERT_EQUAL_INT(7, strlen(small_buffer));
  TEST_ASSERT_EQUAL_INT8('\0', small_buffer[7]);
}

void test_Utils_Secrets_Check_Password_should_pass(void) {
  const unsigned char mock_pass_encrypted[] = {0x07, 0x45, 0x10, 0x1f, 0x36, 0x1e, 0x5e, 0x06, 0x00, 0x03, 0x59, 0x1c, 0x0e};
  const char mock_input_correct[] = "m0ck_p4ssw0rd";

  bool result = Utils_Secrets_Check_Password(mock_input_correct, mock_pass_encrypted, sizeof(mock_pass_encrypted));

  TEST_ASSERT_TRUE(result);
}

void test_Utils_Secrets_Check_Password_should_fail(void) {
  const unsigned char mock_pass_encrypted[] = {0x07, 0x45, 0x10, 0x1f, 0x36, 0x1e, 0x5e, 0x06, 0x00, 0x03, 0x59, 0x1c, 0x0e};
  const char mock_input_incorrect[] = "th1s_1s_n0t_th3_p4ssw0rd";

  bool result = Utils_Secrets_Check_Password(mock_input_incorrect, mock_pass_encrypted, sizeof(mock_pass_encrypted));

  TEST_ASSERT_FALSE(result);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Utils_Secrets_Decrypt_should_decrypt);
  RUN_TEST(test_Utils_Secrets_Decrypt_should_not_overflow);
  RUN_TEST(test_Utils_Secrets_Check_Password_should_pass);
  RUN_TEST(test_Utils_Secrets_Check_Password_should_fail);

  return UNITY_END();
}
