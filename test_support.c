// gcc -Wall -o test_support test_support.c && ./test_support && rm ./test_support
#include <stdio.h>

// NOTE: This file is intended to be compiled and run on a host processor,
// and the resulting output copied into mu_printf_test.c

/*
 * MAKE_MU_PRINTF_TEST is a macro that generates unit tests.  It is intended
 * to be run on a friendly host platform.  It is defined here only as an easy
 * place to stash the macro definition.  It works like this:
 *   MAKE_MU_PRINTF_TEST("%5.6s", "abcdefghijk");
 *   MAKE_MU_PRINTF_TEST("%+.4f", 12.34);
 * emits
 *   MU_TEST(mu_printf(test_printer, NULL, "%5.6s", "abcdefghijk") == 6);
 *   MU_TEST(check_test_emitter("abcdef"));
 *
 *   MU_TEST(mu_printf(test_printer, NULL, "%+.4f", 12.34) == 8);
 *   MU_TEST(check_test_emitter("+12.3400"));
 *
 * which can be copied and pasted into the unit test suite
 */

#define MAKE_MU_PRINTF_TEST(fmt, arg) {\
  char buf[80]; \
  int len = sprintf(buf, fmt, arg); \
  printf("MU_TEST(mu_printf(test_emitter, NULL, \"%s\", ", fmt); \
  printf( #arg ); \
  printf(") == %d);\n", len); \
  printf("MU_TEST(check_test_emitter(\"%s\"));\n\n", buf); \
  }

int main() {

  MAKE_MU_PRINTF_TEST("%f", 0.0);
  MAKE_MU_PRINTF_TEST("% f", 0.0);
  MAKE_MU_PRINTF_TEST("%#f", 0.0);
  MAKE_MU_PRINTF_TEST("%+f", 0.0);
  MAKE_MU_PRINTF_TEST("%-f", 0.0);
  MAKE_MU_PRINTF_TEST("%0f", 0.0);

  MAKE_MU_PRINTF_TEST("%5f", 0.0);
  MAKE_MU_PRINTF_TEST("% 5f", 0.0);
  MAKE_MU_PRINTF_TEST("%#5f", 0.0);
  MAKE_MU_PRINTF_TEST("%+5f", 0.0);
  MAKE_MU_PRINTF_TEST("%-5f", 0.0);
  MAKE_MU_PRINTF_TEST("%05f", 0.0);

  MAKE_MU_PRINTF_TEST("%5.f", 0.0);
  MAKE_MU_PRINTF_TEST("% 5.f", 0.0);
  MAKE_MU_PRINTF_TEST("%#5.f", 0.0);
  MAKE_MU_PRINTF_TEST("%+5.f", 0.0);
  MAKE_MU_PRINTF_TEST("%-5.f", 0.0);
  MAKE_MU_PRINTF_TEST("%05.f", 0.0);

  MAKE_MU_PRINTF_TEST("%5.1f", 0.0);
  MAKE_MU_PRINTF_TEST("% 5.1f", 0.0);
  MAKE_MU_PRINTF_TEST("%#5.1f", 0.0);
  MAKE_MU_PRINTF_TEST("%+5.1f", 0.0);
  MAKE_MU_PRINTF_TEST("%-5.1f", 0.0);
  MAKE_MU_PRINTF_TEST("%05.1f", 0.0);

  MAKE_MU_PRINTF_TEST("%f", 1.0);
  MAKE_MU_PRINTF_TEST("% f", 1.0);
  MAKE_MU_PRINTF_TEST("%#f", 1.0);
  MAKE_MU_PRINTF_TEST("%+f", 1.0);
  MAKE_MU_PRINTF_TEST("%-f", 1.0);
  MAKE_MU_PRINTF_TEST("%0f", 1.0);

  MAKE_MU_PRINTF_TEST("%5f", 1.0);
  MAKE_MU_PRINTF_TEST("% 5f", 1.0);
  MAKE_MU_PRINTF_TEST("%#5f", 1.0);
  MAKE_MU_PRINTF_TEST("%+5f", 1.0);
  MAKE_MU_PRINTF_TEST("%-5f", 1.0);
  MAKE_MU_PRINTF_TEST("%05f", 1.0);

  MAKE_MU_PRINTF_TEST("%5.f", 1.0);
  MAKE_MU_PRINTF_TEST("% 5.f", 1.0);
  MAKE_MU_PRINTF_TEST("%#5.f", 1.0);
  MAKE_MU_PRINTF_TEST("%+5.f", 1.0);
  MAKE_MU_PRINTF_TEST("%-5.f", 1.0);
  MAKE_MU_PRINTF_TEST("%05.f", 1.0);

  MAKE_MU_PRINTF_TEST("%5.1f", 1.0);
  MAKE_MU_PRINTF_TEST("% 5.1f", 1.0);
  MAKE_MU_PRINTF_TEST("%#5.1f", 1.0);
  MAKE_MU_PRINTF_TEST("%+5.1f", 1.0);
  MAKE_MU_PRINTF_TEST("%-5.1f", 1.0);
  MAKE_MU_PRINTF_TEST("%05.1f", 1.0);

  MAKE_MU_PRINTF_TEST("%f", -1.0);
  MAKE_MU_PRINTF_TEST("% f", -1.0);
  MAKE_MU_PRINTF_TEST("%#f", -1.0);
  MAKE_MU_PRINTF_TEST("%+f", -1.0);
  MAKE_MU_PRINTF_TEST("%-f", -1.0);
  MAKE_MU_PRINTF_TEST("%0f", -1.0);

  MAKE_MU_PRINTF_TEST("%5f", -1.0);
  MAKE_MU_PRINTF_TEST("% 5f", -1.0);
  MAKE_MU_PRINTF_TEST("%#5f", -1.0);
  MAKE_MU_PRINTF_TEST("%+5f", -1.0);
  MAKE_MU_PRINTF_TEST("%-5f", -1.0);
  MAKE_MU_PRINTF_TEST("%05f", -1.0);

  MAKE_MU_PRINTF_TEST("%5.f", -1.0);
  MAKE_MU_PRINTF_TEST("% 5.f", -1.0);
  MAKE_MU_PRINTF_TEST("%#5.f", -1.0);
  MAKE_MU_PRINTF_TEST("%+5.f", -1.0);
  MAKE_MU_PRINTF_TEST("%-5.f", -1.0);
  MAKE_MU_PRINTF_TEST("%05.f", -1.0);

  MAKE_MU_PRINTF_TEST("%5.1f", -1.0);
  MAKE_MU_PRINTF_TEST("% 5.1f", -1.0);
  MAKE_MU_PRINTF_TEST("%#5.1f", -1.0);
  MAKE_MU_PRINTF_TEST("%+5.1f", -1.0);
  MAKE_MU_PRINTF_TEST("%-5.1f", -1.0);
  MAKE_MU_PRINTF_TEST("%05.1f", -1.0);

  MAKE_MU_PRINTF_TEST("%f", 9.99);
  MAKE_MU_PRINTF_TEST("% f", 9.99);
  MAKE_MU_PRINTF_TEST("%#f", 9.99);
  MAKE_MU_PRINTF_TEST("%+f", 9.99);
  MAKE_MU_PRINTF_TEST("%-f", 9.99);
  MAKE_MU_PRINTF_TEST("%0f", 9.99);

  MAKE_MU_PRINTF_TEST("%5f", 9.99);
  MAKE_MU_PRINTF_TEST("% 5f", 9.99);
  MAKE_MU_PRINTF_TEST("%#5f", 9.99);
  MAKE_MU_PRINTF_TEST("%+5f", 9.99);
  MAKE_MU_PRINTF_TEST("%-5f", 9.99);
  MAKE_MU_PRINTF_TEST("%05f", 9.99);

  MAKE_MU_PRINTF_TEST("%5.f", 9.99);
  MAKE_MU_PRINTF_TEST("% 5.f", 9.99);
  MAKE_MU_PRINTF_TEST("%#5.f", 9.99);
  MAKE_MU_PRINTF_TEST("%+5.f", 9.99);
  MAKE_MU_PRINTF_TEST("%-5.f", 9.99);
  MAKE_MU_PRINTF_TEST("%05.f", 9.99);

  MAKE_MU_PRINTF_TEST("%5.1f", 9.99);
  MAKE_MU_PRINTF_TEST("% 5.1f", 9.99);
  MAKE_MU_PRINTF_TEST("%#5.1f", 9.99);
  MAKE_MU_PRINTF_TEST("%+5.1f", 9.99);
  MAKE_MU_PRINTF_TEST("%-5.1f", 9.99);
  MAKE_MU_PRINTF_TEST("%05.1f", 9.99);

return 0;
}
