/*
 * muprintf_test.c
 *
 *  Created on: May 14, 2017
 *      Author: r
 */

#include <mu_printf.h>
#include "fsl_debug_console.h"
#include <stddef.h>
#include <string.h>

// ======================================================================
// test support

#define MU_TEST(expr) mu_test((expr), __FILE__, __LINE__, #expr)

void mu_test(bool pass, const char *file, int line, const char *expr) {
  if (pass) {
    // PRINTF("%s:%d: ", file, line);
    // PRINTF("...pass\r\n");
  } else {
    PRINTF("%s:%d: ", file, line);
    PRINTF("...fail %s\r\n", expr);
  }
}

// ======================================================================
// test_emitter support

char test_buf[80];
int test_index = 0;

int test_emitter(void *obj, char ch) {
  test_buf[test_index++] = ch;
  return 1;
}

/*!
 * @brief Return true if test_emitter produced expected results.
 *
 * NOTE: This adds a null to the test_emitter string AND resets the
 * buffer pointer.  In other words, it should only be called once at
 * the end of an individual unit test.
 */
bool check_test_emitter(const char *expected) {
  test_buf[test_index] = '\0';
  bool match = strcmp(test_buf, expected) == 0;
  test_index = 0;
  return match;
}

// ======================================================================
// The unit tests

void mu_null_emitter_test() {
  MU_TEST(mu_null_emitter(NULL, 'a') == 1);
}

void mu_pad_test() {
  PRINTF("...mu_pad_test\r\n");
  MU_TEST(mu_pad(test_emitter, NULL, -1, ' ') == 0);
  MU_TEST(check_test_emitter(""));
  MU_TEST(mu_pad(test_emitter, NULL, 0, ' ') == 0);
  MU_TEST(check_test_emitter(""));
  MU_TEST(mu_pad(test_emitter, NULL, 1, ' ') == 1);
  MU_TEST(check_test_emitter(" "));
  MU_TEST(mu_pad(test_emitter, NULL, 2, '|') == 2);
  MU_TEST(check_test_emitter("||"));
}

void mu_log10_test() {
  PRINTF("...mu_log10_test\r\n");
  MU_TEST(mu_log10(0.10) == -1);
  MU_TEST(mu_log10(0.99) == -1);
  MU_TEST(mu_log10(1.00) == 0);
  MU_TEST(mu_log10(9.95) == 0);
  MU_TEST(mu_log10(10.0) == 1);
  MU_TEST(mu_log10(99.5) == 1);
  MU_TEST(mu_log10(100.) == 2);
}

void mu_pow10_test() {
/*
 * NOTE: exact match testing doesn't work here -- need a "near" test for floats
  MU_TEST(mu_pow10(-6) == 0.000001);
  MU_TEST(mu_pow10(-5) == 0.00001);
  MU_TEST(mu_pow10(-4) == 0.0001);
  MU_TEST(mu_pow10(-3) == 0.001);
  MU_TEST(mu_pow10(-2) == 0.01);
  MU_TEST(mu_pow10(-1) == 0.1);
*/
  PRINTF("...mu_pow10_test\r\n");
  MU_TEST(mu_pow10(0) == 1.0);
  MU_TEST(mu_pow10(1) == 10.0);
  MU_TEST(mu_pow10(2) == 100.0);
  MU_TEST(mu_pow10(3) == 1000.0);
  MU_TEST(mu_pow10(4) == 10000.0);
  MU_TEST(mu_pow10(5) == 100000.0);
  MU_TEST(mu_pow10(6) == 1000000.0);
}
void mu_puti_test() {
  /*!
   * @brief Print a formatted integer
   *
   * Note that if base is 10, v is treated as a signed value, otherwise
   * unsigned.
   *
   * @param emitter_fn Pointer to char emitter function
   * @param obj Pointer-sized user specified arg passed to emitter_fn
   * @param v Integer value to print.  Assumed unsigned.
   * @param base Base in which to print the integer.
   * @param upper If true, print alphanumeric chars upper case
   * @return Number of chars printed.
   */
  PRINTF("...mu_puti_test\r\n");

  MU_TEST(mu_puti(test_emitter, NULL, 0, 10, false) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_puti(test_emitter, NULL, 1, 10, false) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_puti(test_emitter, NULL, -1, 10, false) == 10);
  MU_TEST(check_test_emitter("4294967295"));

  MU_TEST(mu_puti(test_emitter, NULL, 100, 2, false) == 7);
  MU_TEST(check_test_emitter("1100100"));

  MU_TEST(mu_puti(test_emitter, NULL, -100, 2, false) == 32);
  MU_TEST(check_test_emitter("11111111111111111111111110011100"));

  MU_TEST(mu_puti(test_emitter, NULL, 26, 16, false) == 2);
  MU_TEST(check_test_emitter("1a"));

  MU_TEST(mu_puti(test_emitter, NULL, -26, 16, false) == 8);
  MU_TEST(check_test_emitter("ffffffe6"));

  MU_TEST(mu_puti(test_emitter, NULL, 26, 16, true) == 2);
  MU_TEST(check_test_emitter("1A"));

  MU_TEST(mu_puti(test_emitter, NULL, -26, 16, true) == 8);
  MU_TEST(check_test_emitter("FFFFFFE6"));
}

void mu_putf_test() {
  /*!
   * @brief Print a formatted float
   *
   * @param emitter_fn Pointer to char emitter function
   * @param obj Pointer-sized user specified arg passed to emitter_fn
   * @param v Float value to print.  Must be non-negative.
   * @param precision Number of digits to follow decimal point. 0 => no .
   * @return Number of chars printed.
   */
  PRINTF("...mu_putf_test\r\n");

  MU_TEST(mu_putf(test_emitter, NULL, 0.0, 0) == 1);
  MU_TEST(check_test_emitter("0"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.0, 1) == 3);
  MU_TEST(check_test_emitter("0.0"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.0, 6) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_putf(test_emitter, NULL, 1.0, 0) == 1);
  MU_TEST(check_test_emitter("1"));
  MU_TEST(mu_putf(test_emitter, NULL, 1.0, 1) == 3);
  MU_TEST(check_test_emitter("1.0"));
  MU_TEST(mu_putf(test_emitter, NULL, 1.0, 6) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_putf(test_emitter, NULL, 0.0000001, 6) == 8);
  MU_TEST(check_test_emitter("0.000000"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.000001, 6) == 8);
  MU_TEST(check_test_emitter("0.000001"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.00001, 6) == 8);
  MU_TEST(check_test_emitter("0.000010"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.0001, 6) == 8);
  MU_TEST(check_test_emitter("0.000100"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.001, 6) == 8);
  MU_TEST(check_test_emitter("0.001000"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.01, 6) == 8);
  MU_TEST(check_test_emitter("0.010000"));
  MU_TEST(mu_putf(test_emitter, NULL, 0.1, 6) == 8);
  MU_TEST(check_test_emitter("0.100000"));
  MU_TEST(mu_putf(test_emitter, NULL, 10.0, 6) == 9);
  MU_TEST(check_test_emitter("10.000000"));
  MU_TEST(mu_putf(test_emitter, NULL, 100.0, 6) == 10);
  MU_TEST(check_test_emitter("100.000000"));
  MU_TEST(mu_putf(test_emitter, NULL, 1000.0, 6) == 11);
  MU_TEST(check_test_emitter("1000.000000"));
  // 10000 cannot be represented exactly - it's really 10000.0001 so this fails.
  //MU_TEST(mu_putf(test_emitter, NULL, 10000.0, 6) == 12);
  //MU_TEST(check_test_emitter("10000.000000"));
  //MU_TEST(mu_putf(test_emitter, NULL, 100000.0, 6) == 13);
  //MU_TEST(check_test_emitter("100000.000000"));

  MU_TEST(mu_putf(test_emitter, NULL, 6.6666666, 6) == 8);
  MU_TEST(check_test_emitter("6.666667"));
  MU_TEST(mu_putf(test_emitter, NULL, 6.6666666, 5) == 7);
  MU_TEST(check_test_emitter("6.66667"));
  MU_TEST(mu_putf(test_emitter, NULL, 6.6666666, 4) == 6);
  MU_TEST(check_test_emitter("6.6667"));
  MU_TEST(mu_putf(test_emitter, NULL, 6.6666666, 3) == 5);
  MU_TEST(check_test_emitter("6.667"));
  MU_TEST(mu_putf(test_emitter, NULL, 6.6666666, 2) == 4);
  MU_TEST(check_test_emitter("6.67"));
  MU_TEST(mu_putf(test_emitter, NULL, 6.6666666, 1) == 3);
  MU_TEST(check_test_emitter("6.7"));
  MU_TEST(mu_putf(test_emitter, NULL, 6.6666666, 0) == 1);
  MU_TEST(check_test_emitter("7"));

  MU_TEST(mu_putf(test_emitter, NULL, 9.99, 1) == 4);
  MU_TEST(check_test_emitter("10.0"));
}

void mu_printf_c_test() {
  PRINTF("...mu_printf_c_test\r\n");

  MU_TEST(mu_printf(test_emitter, NULL, "%%") == 1);
  MU_TEST(check_test_emitter("%"));

  MU_TEST(mu_printf(test_emitter, NULL, "%c", 'a') == 1);
  MU_TEST(check_test_emitter("a"));

  // %c argument is converted to unsigned int.  -159 == 97 == 'a'
  MU_TEST(mu_printf(test_emitter, NULL, "%c", -159) == 1);
  MU_TEST(check_test_emitter("a"));
}

void mu_printf_s_test() {
  PRINTF("...mu_printf_s_test\r\n");
  MU_TEST(mu_printf(test_emitter, NULL, "%15.0s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("               "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15.0s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("               "));

  MU_TEST(mu_printf(test_emitter, NULL, "%15.18s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("    abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15.18s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("abcdefghijk    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%15.6s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("         abcdef"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15.6s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("abcdef         "));

  MU_TEST(mu_printf(test_emitter, NULL, "%15s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("    abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15s", "abcdefghijk") == 15);
  MU_TEST(check_test_emitter("abcdefghijk    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.0s", "abcdefghijk") == 5);
  MU_TEST(check_test_emitter("     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.0s", "abcdefghijk") == 5);
  MU_TEST(check_test_emitter("     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.18s", "abcdefghijk") == 11);
  MU_TEST(check_test_emitter("abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "-%5.18s", "abcdefghijk") == 12);
  MU_TEST(check_test_emitter("-abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.6s", "abcdefghijk") == 6);
  MU_TEST(check_test_emitter("abcdef"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.6s", "abcdefghijk") == 6);
  MU_TEST(check_test_emitter("abcdef"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5s", "abcdefghijk") == 11);
  MU_TEST(check_test_emitter("abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5s", "abcdefghijk") == 11);
  MU_TEST(check_test_emitter("abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "%s", "abcdefghijk") == 11);
  MU_TEST(check_test_emitter("abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-s", "abcdefghijk") == 11);
  MU_TEST(check_test_emitter("abcdefghijk"));


  MU_TEST(mu_printf(test_emitter, NULL, "%15.0s", "abcd") == 15);
  MU_TEST(check_test_emitter("               "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15.0s", "abcd") == 15);
  MU_TEST(check_test_emitter("               "));

  MU_TEST(mu_printf(test_emitter, NULL, "%15.18s", "abcd") == 15);
  MU_TEST(check_test_emitter("           abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15.18s", "abcd") == 15);
  MU_TEST(check_test_emitter("abcd           "));

  MU_TEST(mu_printf(test_emitter, NULL, "%15.6s", "abcd") == 15);
  MU_TEST(check_test_emitter("           abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15.6s", "abcd") == 15);
  MU_TEST(check_test_emitter("abcd           "));

  MU_TEST(mu_printf(test_emitter, NULL, "%15s", "abcd") == 15);
  MU_TEST(check_test_emitter("           abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-15s", "abcd") == 15);
  MU_TEST(check_test_emitter("abcd           "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.0s", "abcd") == 5);
  MU_TEST(check_test_emitter("     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.0s", "abcd") == 5);
  MU_TEST(check_test_emitter("     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.18s", "abcd") == 5);
  MU_TEST(check_test_emitter(" abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.18s", "abcd") == 5);
  MU_TEST(check_test_emitter("abcd "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.6s", "abcd") == 5);
  MU_TEST(check_test_emitter(" abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.6s", "abcd") == 5);
  MU_TEST(check_test_emitter(" abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5s", "abcd") == 5);
  MU_TEST(check_test_emitter(" abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5s", "abcd") == 5);
  MU_TEST(check_test_emitter("abcd "));

  MU_TEST(mu_printf(test_emitter, NULL, "%s", "abcd") == 4);
  MU_TEST(check_test_emitter("abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-s", "abcd") == 4);
  MU_TEST(check_test_emitter("abcd"));

}

void mu_printf_d_test() {
  MU_TEST(mu_printf(test_emitter, NULL, "%d", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+d", 0) == 2);
  MU_TEST(check_test_emitter("+0"));

  MU_TEST(mu_printf(test_emitter, NULL, "% d", 0) == 2);
  MU_TEST(check_test_emitter(" 0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-d", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10d", 0) == 10);
  MU_TEST(check_test_emitter("         0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10d", 0) == 10);
  MU_TEST(check_test_emitter("0         "));

  MU_TEST(mu_printf(test_emitter, NULL, "%10.5d", 0) == 10);
  MU_TEST(check_test_emitter("     00000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%d", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%d", -1) == 2);
  MU_TEST(check_test_emitter("-1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10d", 1) == 10);
  MU_TEST(check_test_emitter("         1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10d", -1) == 10);
  MU_TEST(check_test_emitter("        -1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10.5d", 1) == 10);
  MU_TEST(check_test_emitter("     00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10.5d", -1) == 10);
  MU_TEST(check_test_emitter("    -00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0d", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0d", -1) == 2);
  MU_TEST(check_test_emitter("-1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010d", 1) == 10);
  MU_TEST(check_test_emitter("0000000001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010d", -1) == 10);
  MU_TEST(check_test_emitter("-000000001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010.5d", 1) == 10);
  MU_TEST(check_test_emitter("     00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010.5d", -1) == 10);
  MU_TEST(check_test_emitter("    -00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-d", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-d", -1) == 2);
  MU_TEST(check_test_emitter("-1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10d", 1) == 10);
  MU_TEST(check_test_emitter("1         "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10d", -1) == 10);
  MU_TEST(check_test_emitter("-1        "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10.5d", 1) == 10);
  MU_TEST(check_test_emitter("00001     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10.5d", -1) == 10);
  MU_TEST(check_test_emitter("-00001    "));

  MU_TEST(mu_printf(test_emitter, NULL, "% d", 1) == 2);
  MU_TEST(check_test_emitter(" 1"));

  MU_TEST(mu_printf(test_emitter, NULL, "% d", -1) == 2);
  MU_TEST(check_test_emitter("-1"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10d", 1) == 10);
  MU_TEST(check_test_emitter("         1"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10d", -1) == 10);
  MU_TEST(check_test_emitter("        -1"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10.5d", 1) == 10);
  MU_TEST(check_test_emitter("     00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10.5d", -1) == 10);
  MU_TEST(check_test_emitter("    -00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+d", 1) == 2);
  MU_TEST(check_test_emitter("+1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+d", -1) == 2);
  MU_TEST(check_test_emitter("-1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10d", 1) == 10);
  MU_TEST(check_test_emitter("        +1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10d", -1) == 10);
  MU_TEST(check_test_emitter("        -1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10.5d", 1) == 10);
  MU_TEST(check_test_emitter("    +00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10.5d", -1) == 10);
  MU_TEST(check_test_emitter("    -00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%d", 123456) == 6);
  MU_TEST(check_test_emitter("123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%d", -123456) == 7);
  MU_TEST(check_test_emitter("-123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10d", 123456) == 10);
  MU_TEST(check_test_emitter("    123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10d", -123456) == 10);
  MU_TEST(check_test_emitter("   -123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10.5d", 123456) == 10);
  MU_TEST(check_test_emitter("    123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%10.5d", -123456) == 10);
  MU_TEST(check_test_emitter("   -123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0d", 123456) == 6);
  MU_TEST(check_test_emitter("123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0d", -123456) == 7);
  MU_TEST(check_test_emitter("-123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010d", 123456) == 10);
  MU_TEST(check_test_emitter("0000123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010d", -123456) == 10);
  MU_TEST(check_test_emitter("-000123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010.5d", 123456) == 10);
  MU_TEST(check_test_emitter("    123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010.5d", -123456) == 10);
  MU_TEST(check_test_emitter("   -123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-d", 123456) == 6);
  MU_TEST(check_test_emitter("123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-d", -123456) == 7);
  MU_TEST(check_test_emitter("-123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10d", 123456) == 10);
  MU_TEST(check_test_emitter("123456    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10d", -123456) == 10);
  MU_TEST(check_test_emitter("-123456   "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10.5d", 123456) == 10);
  MU_TEST(check_test_emitter("123456    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-10.5d", -123456) == 10);
  MU_TEST(check_test_emitter("-123456   "));

  MU_TEST(mu_printf(test_emitter, NULL, "% d", 123456) == 7);
  MU_TEST(check_test_emitter(" 123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "% d", -123456) == 7);
  MU_TEST(check_test_emitter("-123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10d", 123456) == 10);
  MU_TEST(check_test_emitter("    123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10d", -123456) == 10);
  MU_TEST(check_test_emitter("   -123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10.5d", 123456) == 10);
  MU_TEST(check_test_emitter("    123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 10.5d", -123456) == 10);
  MU_TEST(check_test_emitter("   -123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+d", 123456) == 7);
  MU_TEST(check_test_emitter("+123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+d", -123456) == 7);
  MU_TEST(check_test_emitter("-123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10d", 123456) == 10);
  MU_TEST(check_test_emitter("   +123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10d", -123456) == 10);
  MU_TEST(check_test_emitter("   -123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10.5d", 123456) == 10);
  MU_TEST(check_test_emitter("   +123456"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+10.5d", -123456) == 10);
  MU_TEST(check_test_emitter("   -123456"));
}

void mu_printf_test() {
  PRINTF("begin tests...\r\n");
  mu_null_emitter_test();
  mu_pad_test();
  mu_log10_test();
  mu_pow10_test();
  mu_puti_test();
  mu_putf_test();
  mu_printf_c_test();
  mu_printf_s_test();
  mu_printf_d_test();
  PRINTF("...end of tests\r\n");
}
