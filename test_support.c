/*
 * mu_unit_test_definitions.c
 *
 *  Created on: May 23, 2017
 *      Author: r
 */

// gcc -Wall -o test_support test_support.c && ./test_support
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
  // strings
  // Theory: length = max(width, max(strlen, precision))
  MAKE_MU_PRINTF_TEST("%-15.0s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-15.18s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-15.6s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-15.s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-15s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-5.0s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-5.6s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-5.s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-5s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%-s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%15.0s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%15.18s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%15.6s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%15.s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%15s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%5.0s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%5.18s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%5.6s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%5.s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%5s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("%s", "abcdefghijk");
  MAKE_MU_PRINTF_TEST("-%5.18s", "abcdefghijk");
  printf("printf(\"\\n\");\n\n");

  MAKE_MU_PRINTF_TEST("%-15.0s", "abcd");
  MAKE_MU_PRINTF_TEST("%-15.18s", "abcd");
  MAKE_MU_PRINTF_TEST("%-15.6s", "abcd");
  MAKE_MU_PRINTF_TEST("%-15.s", "abcd");
  MAKE_MU_PRINTF_TEST("%-15s", "abcd");
  MAKE_MU_PRINTF_TEST("%-5.0s", "abcd");
  MAKE_MU_PRINTF_TEST("%-5.18s", "abcd");
  MAKE_MU_PRINTF_TEST("%-5.s", "abcd");
  MAKE_MU_PRINTF_TEST("%-5s", "abcd");
  MAKE_MU_PRINTF_TEST("%-s", "abcd");
  MAKE_MU_PRINTF_TEST("%15.0s", "abcd");
  MAKE_MU_PRINTF_TEST("%15.18s", "abcd");
  MAKE_MU_PRINTF_TEST("%15.6s", "abcd");
  MAKE_MU_PRINTF_TEST("%15.s", "abcd");
  MAKE_MU_PRINTF_TEST("%15s", "abcd");
  MAKE_MU_PRINTF_TEST("%5.0s", "abcd");
  MAKE_MU_PRINTF_TEST("%5.18s", "abcd");
  MAKE_MU_PRINTF_TEST("%5.6s", "abcd");
  MAKE_MU_PRINTF_TEST("%5.6s", "abcd");
  MAKE_MU_PRINTF_TEST("%5.s", "abcd");
  MAKE_MU_PRINTF_TEST("%5s", "abcd");
  MAKE_MU_PRINTF_TEST("%s", "abcd");
  printf("printf(\"\\n\");\n\n");

  return 0;
}
