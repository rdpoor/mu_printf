/*
 * muprintf_test.c
 *
 *  Created on: May 14, 2017
 *      Author: r
 *
 * To compile standalone:
 * gcc -DSTANDALONE -Wall -o mu_printf_test mu_printf_test.c mu_printf.c && ./mu_printf_test
 */

#ifdef STANDALONE
#include <stdio.h>
#define PRINTF printf
#else
#include "fsl_debug_console.h"
#endif

#include "mu_printf.h"
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

bool nearly_equal(float f1, float f2, float e) {
  return ((f1 - e) <= f2) && ((f1 + e) >= f2);
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

void mu_precision_test() {
  PRINTF("...mu_precision_test\r\n");
  MU_TEST(mu_precision(0.0, 0) == 0);

  MU_TEST(nearly_equal(mu_precision(1.111111, 6), 1.111111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(1.111111, 5), 1.11111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(1.111111, 4), 1.1111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(1.111111, 3), 1.111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(1.111111, 2), 1.11, 0.000001));
  MU_TEST(nearly_equal(mu_precision(1.111111, 1), 1.1, 0.000001));
  MU_TEST(nearly_equal(mu_precision(1.111111, 0), 1.0, 0.000001));
  
  MU_TEST(nearly_equal(mu_precision(5.555555, 6), 5.555555, 0.000001));
  MU_TEST(nearly_equal(mu_precision(5.555555, 5), 5.55556, 0.000001));
  MU_TEST(nearly_equal(mu_precision(5.555555, 4), 5.5556, 0.000001));
  MU_TEST(nearly_equal(mu_precision(5.555555, 3), 5.556, 0.000001));
  MU_TEST(nearly_equal(mu_precision(5.555555, 2), 5.56, 0.000001));
  MU_TEST(nearly_equal(mu_precision(5.555555, 1), 5.6, 0.000001));
  MU_TEST(nearly_equal(mu_precision(5.555555, 0), 6.0, 0.000001));
  
  MU_TEST(nearly_equal(mu_precision(9.999999, 6), 9.999999, 0.000001));
  MU_TEST(nearly_equal(mu_precision(9.999999, 5), 10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(9.999999, 4), 10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(9.999999, 3), 10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(9.999999, 2), 10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(9.999999, 1), 10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(9.999999, 0), 10.0, 0.000001));
  
  MU_TEST(nearly_equal(mu_precision(-1.111111, 6), -1.111111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-1.111111, 5), -1.11111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-1.111111, 4), -1.1111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-1.111111, 3), -1.111, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-1.111111, 2), -1.11, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-1.111111, 1), -1.1, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-1.111111, 0), -1.0, 0.000001));
  
  MU_TEST(nearly_equal(mu_precision(-5.555555, 6), -5.555555, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-5.555555, 5), -5.55556, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-5.555555, 4), -5.5556, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-5.555555, 3), -5.556, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-5.555555, 2), -5.56, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-5.555555, 1), -5.6, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-5.555555, 0), -6.0, 0.000001));
  
  MU_TEST(nearly_equal(mu_precision(-9.999999, 6), -9.999999, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-9.999999, 5), -10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-9.999999, 4), -10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-9.999999, 3), -10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-9.999999, 2), -10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-9.999999, 1), -10.0, 0.000001));
  MU_TEST(nearly_equal(mu_precision(-9.999999, 0), -10.0, 0.000001));
}

void mu_pad_test() {
  PRINTF("...mu_pad_test\r\n");
  MU_TEST(mu_emit_pad(test_emitter, NULL, ' ', -1) == 0);
  MU_TEST(check_test_emitter(""));
  MU_TEST(mu_emit_pad(test_emitter, NULL, ' ', 0) == 0);
  MU_TEST(check_test_emitter(""));
  MU_TEST(mu_emit_pad(test_emitter, NULL, ' ', 1) == 1);
  MU_TEST(check_test_emitter(" "));
  MU_TEST(mu_emit_pad(test_emitter, NULL, '|', 2) == 2);
  MU_TEST(check_test_emitter("||"));
}

void mu_floor_log10_test() {
  PRINTF("...mu_floor_log10_test\r\n");
  MU_TEST(mu_floor_log10(0.10) == -1);
  MU_TEST(mu_floor_log10(0.99) == -1);
  MU_TEST(mu_floor_log10(1.00) == 0);
  MU_TEST(mu_floor_log10(9.95) == 0);
  MU_TEST(mu_floor_log10(10.0) == 1);
  MU_TEST(mu_floor_log10(99.5) == 1);
  MU_TEST(mu_floor_log10(100.) == 2);
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

  MU_TEST(mu_emit_integer(test_emitter, NULL, 0, 10, false) == 0);
  MU_TEST(check_test_emitter(""));

  MU_TEST(mu_emit_integer(test_emitter, NULL, 1, 10, false) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_emit_integer(test_emitter, NULL, -1, 10, false) == 10);
  MU_TEST(check_test_emitter("4294967295"));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_emit_integer(test_emitter, NULL, 100, 2, false) == 7);
  MU_TEST(check_test_emitter("1100100"));

  MU_TEST(mu_emit_integer(test_emitter, NULL, -100, 2, false) == 32);
  MU_TEST(check_test_emitter("11111111111111111111111110011100"));

  MU_TEST(mu_emit_integer(test_emitter, NULL, 26, 16, false) == 2);
  MU_TEST(check_test_emitter("1a"));

  MU_TEST(mu_emit_integer(test_emitter, NULL, -26, 16, false) == 8);
  MU_TEST(check_test_emitter("ffffffe6"));
  // PRINTF("got '%s'\r\n", test_buf);


  MU_TEST(mu_emit_integer(test_emitter, NULL, 26, 16, true) == 2);
  MU_TEST(check_test_emitter("1A"));

  MU_TEST(mu_emit_integer(test_emitter, NULL, -26, 16, true) == 8);
  MU_TEST(check_test_emitter("FFFFFFE6"));
  // PRINTF("got '%s'\r\n", test_buf);
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

  MU_TEST(mu_emit_float(test_emitter, NULL, 0.0, 0) == 1);
  MU_TEST(check_test_emitter("0"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.0, 1) == 3);
  MU_TEST(check_test_emitter("0.0"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.0, 6) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_emit_float(test_emitter, NULL, 1.0, 0) == 1);
  MU_TEST(check_test_emitter("1"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 1.0, 1) == 3);
  MU_TEST(check_test_emitter("1.0"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 1.0, 6) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_emit_float(test_emitter, NULL, 0.0000001, 6) == 8);
  MU_TEST(check_test_emitter("0.000000"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.000001, 6) == 8);
  MU_TEST(check_test_emitter("0.000001"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.00001, 6) == 8);
  MU_TEST(check_test_emitter("0.000010"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.0001, 6) == 8);
  MU_TEST(check_test_emitter("0.000100"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.001, 6) == 8);
  MU_TEST(check_test_emitter("0.001000"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.01, 6) == 8);
  MU_TEST(check_test_emitter("0.010000"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 0.1, 6) == 8);
  MU_TEST(check_test_emitter("0.100000"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 10.0, 6) == 9);
  MU_TEST(check_test_emitter("10.000000"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 100.0, 6) == 10);
  MU_TEST(check_test_emitter("100.000000"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 1000.0, 6) == 11);
  MU_TEST(check_test_emitter("1000.000000"));
  // 10000 cannot be represented exactly - it's really 10000.0001 so this fails.
  //MU_TEST(mu_putf(test_emitter, NULL, 10000.0, 6) == 12);
  //MU_TEST(check_test_emitter("10000.000000"));
  //MU_TEST(mu_putf(test_emitter, NULL, 100000.0, 6) == 13);
  //MU_TEST(check_test_emitter("100000.000000"));

  MU_TEST(mu_emit_float(test_emitter, NULL, 6.6666666, 6) == 8);
  MU_TEST(check_test_emitter("6.666667"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 6.6666666, 5) == 7);
  MU_TEST(check_test_emitter("6.66667"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 6.6666666, 4) == 6);
  MU_TEST(check_test_emitter("6.6667"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 6.6666666, 3) == 5);
  MU_TEST(check_test_emitter("6.667"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 6.6666666, 2) == 4);
  MU_TEST(check_test_emitter("6.67"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 6.6666666, 1) == 3);
  MU_TEST(check_test_emitter("6.7"));
  MU_TEST(mu_emit_float(test_emitter, NULL, 6.6666666, 0) == 1);
  MU_TEST(check_test_emitter("7"));

  MU_TEST(mu_emit_float(test_emitter, NULL, 9.99, 1) == 4);
  MU_TEST(check_test_emitter("10.0"));
}

void mu_parse_directive_test() {
  mu_directive_t directive;

  MU_TEST(*mu_parse_directive(&directive, "a?") == '?');
  MU_TEST(directive.flags.all == 0);
  MU_TEST(directive.width == 0);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "1a?") == '?');
  MU_TEST(directive.flags.all == 0);
  MU_TEST(directive.width == 1);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "#a?") == '?');
  MU_TEST(directive.flags.alternate_form != 0);
  MU_TEST(directive.width == 0);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "+a?") == '?');
  MU_TEST(directive.flags.pad_plus != 0);
  MU_TEST(directive.width == 0);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "-a?") == '?');
  MU_TEST(directive.flags.pad_right != 0);
  MU_TEST(directive.width == 0);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, " a?") == '?');
  MU_TEST(directive.flags.pad_space != 0);
  MU_TEST(directive.width == 0);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "0a?") == '?');
  MU_TEST(directive.flags.pad_zero != 0);
  MU_TEST(directive.width == 0);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "01a?") == '?');
  MU_TEST(directive.flags.pad_zero != 0);
  MU_TEST(directive.width == 1);
  MU_TEST(directive.precision == MU_PRECISION_NOT_GIVEN);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "01.a?") == '?');
  MU_TEST(directive.flags.pad_zero != 0);
  MU_TEST(directive.width == 1);
  MU_TEST(directive.precision == 0);
  MU_TEST(directive.conversion == 'a');

  MU_TEST(*mu_parse_directive(&directive, "01.2a?") == '?');
  MU_TEST(directive.flags.pad_zero != 0);
  MU_TEST(directive.width == 1);
  MU_TEST(directive.precision == 2);
  MU_TEST(directive.conversion == 'a');

  // mutual exclusions
  MU_TEST(*mu_parse_directive(&directive, "0-1.2a?") == '?');
  MU_TEST(directive.flags.pad_zero == 0);
  MU_TEST(directive.flags.pad_right != 0);

  MU_TEST(*mu_parse_directive(&directive, "+ 1.2a?") == '?');
  MU_TEST(directive.flags.pad_plus != 0);
  MU_TEST(directive.flags.pad_space == 0);

  MU_TEST(*mu_parse_directive(&directive, "01.2A?") == '?');
  MU_TEST(directive.flags.upper_case != 0);
  MU_TEST(directive.flags.pad_zero != 0);
  MU_TEST(directive.width == 1);
  MU_TEST(directive.precision == 2);
  MU_TEST(directive.conversion == 'a');
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

  MU_TEST(mu_printf(test_emitter, NULL, "%s", "abcdefghijk") == 11);
  MU_TEST(check_test_emitter("abcdefghijk"));

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

  MU_TEST(mu_printf(test_emitter, NULL, "%-s", "abcdefghijk") == 11);
  MU_TEST(check_test_emitter("abcdefghijk"));

  MU_TEST(mu_printf(test_emitter, NULL, "%010s", "abcd") == 10);
  MU_TEST(check_test_emitter("      abcd"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-010s", "abcd") == 10);
  MU_TEST(check_test_emitter("abcd      "));

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
  PRINTF("...mu_printf_d_test\r\n");

  MU_TEST(mu_printf(test_emitter, NULL, "%d", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%d", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%d", -1) == 2);
  MU_TEST(check_test_emitter("-1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%.d", 0) == 0);
  MU_TEST(check_test_emitter(""));

  MU_TEST(mu_printf(test_emitter, NULL, "%.d", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%.d", -1) == 2);
  MU_TEST(check_test_emitter("-1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%.3d", 0) == 3);
  MU_TEST(check_test_emitter("000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%.3d", 1) == 3);
  MU_TEST(check_test_emitter("001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%.3d", -1) == 4);
  MU_TEST(check_test_emitter("-001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+d", 1) == 2);
  MU_TEST(check_test_emitter("+1"));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "% d", 1) == 2);
  MU_TEST(check_test_emitter(" 1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5d", 1) == 5);
  MU_TEST(check_test_emitter("   +1"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5d", 1) == 5);
  MU_TEST(check_test_emitter("    1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05d", 1) == 5);
  MU_TEST(check_test_emitter("00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5d", 1) == 5);
  MU_TEST(check_test_emitter("1    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%d", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+d", 0) == 2);
  MU_TEST(check_test_emitter("+0"));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "% d", 0) == 2);
  MU_TEST(check_test_emitter(" 0"));
  // PRINTF("got '%s'\r\n", test_buf);

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

void mu_printf_u_test() {
  PRINTF("...mu_printf_xoX_test\r\n");
  MU_TEST(mu_printf(test_emitter, NULL, "%x", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#x", 0) == 1);
  MU_TEST(check_test_emitter("0"));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "%0x", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-x", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5x", 0) == 5);
  MU_TEST(check_test_emitter("    0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5x", 0) == 5);
  MU_TEST(check_test_emitter("    0"));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "%05x", 0) == 5);
  MU_TEST(check_test_emitter("00000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5x", 0) == 5);
  MU_TEST(check_test_emitter("0    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.x", 0) == 5);
  MU_TEST(check_test_emitter("     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.x", 0) == 5);
  MU_TEST(check_test_emitter("     "));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "%05.x", 0) == 5);
  MU_TEST(check_test_emitter("     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.x", 0) == 5);
  MU_TEST(check_test_emitter("     "));

  MU_TEST(mu_printf(test_emitter, NULL, "%x", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#x", 1) == 3);
  MU_TEST(check_test_emitter("0x1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0x", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-x", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5x", 1) == 5);
  MU_TEST(check_test_emitter("    1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5x", 1) == 5);
  MU_TEST(check_test_emitter("  0x1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05x", 1) == 5);
  MU_TEST(check_test_emitter("00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5x", 1) == 5);
  MU_TEST(check_test_emitter("1    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.x", 1) == 5);
  MU_TEST(check_test_emitter("    1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.x", 1) == 5);
  MU_TEST(check_test_emitter("  0x1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.x", 1) == 5);
  MU_TEST(check_test_emitter("    1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.x", 1) == 5);
  MU_TEST(check_test_emitter("1    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%x", 6844) == 4);
  MU_TEST(check_test_emitter("1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#x", 6844) == 6);
  MU_TEST(check_test_emitter("0x1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0x", 6844) == 4);
  MU_TEST(check_test_emitter("1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-x", 6844) == 4);
  MU_TEST(check_test_emitter("1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5x", 6844) == 5);
  MU_TEST(check_test_emitter(" 1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5x", 6844) == 6);
  MU_TEST(check_test_emitter("0x1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05x", 6844) == 5);
  MU_TEST(check_test_emitter("01abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5x", 6844) == 5);
  MU_TEST(check_test_emitter("1abc "));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.x", 6844) == 5);
  MU_TEST(check_test_emitter(" 1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.x", 6844) == 6);
  MU_TEST(check_test_emitter("0x1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.x", 6844) == 5);
  MU_TEST(check_test_emitter(" 1abc"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.x", 6844) == 5);
  MU_TEST(check_test_emitter("1abc "));

  MU_TEST(mu_printf(test_emitter, NULL, "%X", 6844) == 4);
  MU_TEST(check_test_emitter("1ABC"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#X", 6844) == 6);
  MU_TEST(check_test_emitter("0X1ABC"));

  MU_TEST(mu_printf(test_emitter, NULL, "%o", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%o", 6844) == 5);
  MU_TEST(check_test_emitter("15274"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#o", 6844) == 6);
  MU_TEST(check_test_emitter("015274"));

  MU_TEST(mu_printf(test_emitter, NULL, "%b", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#b", 0) == 1);
  MU_TEST(check_test_emitter("0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%b", 1) == 1);
  MU_TEST(check_test_emitter("1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#b", 1) == 3);
  MU_TEST(check_test_emitter("0b1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%b", 90) == 7);
  MU_TEST(check_test_emitter("1011010"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#b", 90) == 9);
  MU_TEST(check_test_emitter("0b1011010"));

}

void mu_printf_f_test() {
  PRINTF("...mu_printf_f_test\r\n");

  MU_TEST(mu_printf(test_emitter, NULL, "%f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% f", 0.0) == 9);
  MU_TEST(check_test_emitter(" 0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+f", 0.0) == 9);
  MU_TEST(check_test_emitter("+0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5f", 0.0) == 9);
  MU_TEST(check_test_emitter(" 0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5f", 0.0) == 9);
  MU_TEST(check_test_emitter("+0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05f", 0.0) == 8);
  MU_TEST(check_test_emitter("0.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.f", 0.0) == 5);
  MU_TEST(check_test_emitter("    0"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.f", 0.0) == 5);
  MU_TEST(check_test_emitter("    0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.f", 0.0) == 5);
  MU_TEST(check_test_emitter("   0."));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.f", 0.0) == 5);
  MU_TEST(check_test_emitter("   +0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.f", 0.0) == 5);
  MU_TEST(check_test_emitter("0    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.f", 0.0) == 5);
  MU_TEST(check_test_emitter("00000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.1f", 0.0) == 5);
  MU_TEST(check_test_emitter("  0.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.1f", 0.0) == 5);
  MU_TEST(check_test_emitter("  0.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.1f", 0.0) == 5);
  MU_TEST(check_test_emitter("  0.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.1f", 0.0) == 5);
  MU_TEST(check_test_emitter(" +0.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.1f", 0.0) == 5);
  MU_TEST(check_test_emitter("0.0  "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.1f", 0.0) == 5);
  MU_TEST(check_test_emitter("000.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% f", 1.0) == 9);
  MU_TEST(check_test_emitter(" 1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+f", 1.0) == 9);
  MU_TEST(check_test_emitter("+1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5f", 1.0) == 9);
  MU_TEST(check_test_emitter(" 1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5f", 1.0) == 9);
  MU_TEST(check_test_emitter("+1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05f", 1.0) == 8);
  MU_TEST(check_test_emitter("1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.f", 1.0) == 5);
  MU_TEST(check_test_emitter("    1"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.f", 1.0) == 5);
  MU_TEST(check_test_emitter("    1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.f", 1.0) == 5);
  MU_TEST(check_test_emitter("   1."));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.f", 1.0) == 5);
  MU_TEST(check_test_emitter("   +1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.f", 1.0) == 5);
  MU_TEST(check_test_emitter("1    "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.f", 1.0) == 5);
  MU_TEST(check_test_emitter("00001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.1f", 1.0) == 5);
  MU_TEST(check_test_emitter("  1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.1f", 1.0) == 5);
  MU_TEST(check_test_emitter("  1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.1f", 1.0) == 5);
  MU_TEST(check_test_emitter("  1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.1f", 1.0) == 5);
  MU_TEST(check_test_emitter(" +1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.1f", 1.0) == 5);
  MU_TEST(check_test_emitter("1.0  "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.1f", 1.0) == 5);
  MU_TEST(check_test_emitter("001.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05f", -1.0) == 9);
  MU_TEST(check_test_emitter("-1.000000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.f", -1.0) == 5);
  MU_TEST(check_test_emitter("   -1"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.f", -1.0) == 5);
  MU_TEST(check_test_emitter("   -1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.f", -1.0) == 5);
  MU_TEST(check_test_emitter("  -1."));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.f", -1.0) == 5);
  MU_TEST(check_test_emitter("   -1"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.f", -1.0) == 5);
  MU_TEST(check_test_emitter("-1   "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.f", -1.0) == 5);
  MU_TEST(check_test_emitter("-0001"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.1f", -1.0) == 5);
  MU_TEST(check_test_emitter(" -1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.1f", -1.0) == 5);
  MU_TEST(check_test_emitter(" -1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.1f", -1.0) == 5);
  MU_TEST(check_test_emitter(" -1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.1f", -1.0) == 5);
  MU_TEST(check_test_emitter(" -1.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.1f", -1.0) == 5);
  MU_TEST(check_test_emitter("-1.0 "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.1f", -1.0) == 5);
  MU_TEST(check_test_emitter("-01.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% f", 9.99) == 9);
  MU_TEST(check_test_emitter(" 9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+f", 9.99) == 9);
  MU_TEST(check_test_emitter("+9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5f", 9.99) == 9);
  MU_TEST(check_test_emitter(" 9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5f", 9.99) == 9);
  MU_TEST(check_test_emitter("+9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05f", 9.99) == 8);
  MU_TEST(check_test_emitter("9.990000"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.f", 9.99) == 5);
  MU_TEST(check_test_emitter("   10"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.f", 9.99) == 5);
  MU_TEST(check_test_emitter("   10"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.f", 9.99) == 5);
  MU_TEST(check_test_emitter("  10."));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.f", 9.99) == 5);
  MU_TEST(check_test_emitter("  +10"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.f", 9.99) == 5);
  MU_TEST(check_test_emitter("10   "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.f", 9.99) == 5);
  MU_TEST(check_test_emitter("00010"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5.1f", 9.99) == 5);
  MU_TEST(check_test_emitter(" 10.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5.1f", 9.99) == 5);
  MU_TEST(check_test_emitter(" 10.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5.1f", 9.99) == 5);
  MU_TEST(check_test_emitter(" 10.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5.1f", 9.99) == 5);
  MU_TEST(check_test_emitter("+10.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5.1f", 9.99) == 5);
  MU_TEST(check_test_emitter("10.0 "));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.1f", 9.99) == 5);
  MU_TEST(check_test_emitter("010.0"));



  MU_TEST(mu_printf(test_emitter, NULL, "%05.1f", 1.0) == 5);
  MU_TEST(check_test_emitter("001.0"));
  // PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "% 05.1f", 1.0) == 5);
  MU_TEST(check_test_emitter(" 01.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+05.1f", 1.0) == 5);
  MU_TEST(check_test_emitter("+01.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05.1f", -1.0) == 5);
  MU_TEST(check_test_emitter("-01.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 05.1f", -1.0) == 5);
  MU_TEST(check_test_emitter("-01.0"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+05.1f", -1.0) == 5);
  MU_TEST(check_test_emitter("-01.0"));

}

void mu_printf_e_test() {
  PRINTF("...mu_printf_e_test\r\n");
  MU_TEST(mu_printf(test_emitter, NULL, "%e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));
  // PRINTF("got '%s'\r\n", test_buf);
  
  MU_TEST(mu_printf(test_emitter, NULL, "% e", 0.0) == 13);
  MU_TEST(check_test_emitter(" 0.000000e+00"));
  
  MU_TEST(mu_printf(test_emitter, NULL, "%#e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));
  
  MU_TEST(mu_printf(test_emitter, NULL, "%+e", 0.0) == 13);
  MU_TEST(check_test_emitter("+0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5e", 0.0) == 13);
  MU_TEST(check_test_emitter(" 0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5e", 0.0) == 13);
  MU_TEST(check_test_emitter("+0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05e", 0.0) == 12);
  MU_TEST(check_test_emitter("0.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.e", 0.0) == 8);
  MU_TEST(check_test_emitter("   0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.e", 0.0) == 8);
  MU_TEST(check_test_emitter("   0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.e", 0.0) == 8);
  MU_TEST(check_test_emitter("  0.e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.e", 0.0) == 8);
  MU_TEST(check_test_emitter("  +0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.e", 0.0) == 8);
  MU_TEST(check_test_emitter("0e+00   "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.e", 0.0) == 8);
  MU_TEST(check_test_emitter("0000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.1e", 0.0) == 8);
  MU_TEST(check_test_emitter(" 0.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.1e", 0.0) == 8);
  MU_TEST(check_test_emitter(" 0.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.1e", 0.0) == 8);
  MU_TEST(check_test_emitter(" 0.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.1e", 0.0) == 8);
  MU_TEST(check_test_emitter("+0.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.1e", 0.0) == 8);
  MU_TEST(check_test_emitter("0.0e+00 "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.1e", 0.0) == 8);
  MU_TEST(check_test_emitter("00.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% e", 1.0) == 13);
  MU_TEST(check_test_emitter(" 1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+e", 1.0) == 13);
  MU_TEST(check_test_emitter("+1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5e", 1.0) == 13);
  MU_TEST(check_test_emitter(" 1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5e", 1.0) == 13);
  MU_TEST(check_test_emitter("+1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05e", 1.0) == 12);
  MU_TEST(check_test_emitter("1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.e", 1.0) == 8);
  MU_TEST(check_test_emitter("   1e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.e", 1.0) == 8);
  MU_TEST(check_test_emitter("   1e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.e", 1.0) == 8);
  MU_TEST(check_test_emitter("  1.e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.e", 1.0) == 8);
  MU_TEST(check_test_emitter("  +1e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.e", 1.0) == 8);
  MU_TEST(check_test_emitter("1e+00   "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.e", 1.0) == 8);
  MU_TEST(check_test_emitter("0001e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.1e", 1.0) == 8);
  MU_TEST(check_test_emitter(" 1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.1e", 1.0) == 8);
  MU_TEST(check_test_emitter(" 1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.1e", 1.0) == 8);
  MU_TEST(check_test_emitter(" 1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.1e", 1.0) == 8);
  MU_TEST(check_test_emitter("+1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.1e", 1.0) == 8);
  MU_TEST(check_test_emitter("1.0e+00 "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.1e", 1.0) == 8);
  MU_TEST(check_test_emitter("01.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05e", -1.0) == 13);
  MU_TEST(check_test_emitter("-1.000000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.e", -1.0) == 8);
  MU_TEST(check_test_emitter("  -1e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.e", -1.0) == 8);
  MU_TEST(check_test_emitter("  -1e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.e", -1.0) == 8);
  MU_TEST(check_test_emitter(" -1.e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.e", -1.0) == 8);
  MU_TEST(check_test_emitter("  -1e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.e", -1.0) == 8);
  MU_TEST(check_test_emitter("-1e+00  "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.e", -1.0) == 8);
  MU_TEST(check_test_emitter("-001e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.1e", -1.0) == 8);
  MU_TEST(check_test_emitter("-1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.1e", -1.0) == 8);
  MU_TEST(check_test_emitter("-1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.1e", -1.0) == 8);
  MU_TEST(check_test_emitter("-1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.1e", -1.0) == 8);
  MU_TEST(check_test_emitter("-1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.1e", -1.0) == 8);
  MU_TEST(check_test_emitter("-1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.1e", -1.0) == 8);
  MU_TEST(check_test_emitter("-1.0e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% e", 9.99) == 13);
  MU_TEST(check_test_emitter(" 9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+e", 9.99) == 13);
  MU_TEST(check_test_emitter("+9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%0e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%5e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 5e", 9.99) == 13);
  MU_TEST(check_test_emitter(" 9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#5e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+5e", 9.99) == 13);
  MU_TEST(check_test_emitter("+9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-5e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%05e", 9.99) == 12);
  MU_TEST(check_test_emitter("9.990000e+00"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.e", 9.99) == 8);
  MU_TEST(check_test_emitter("   1e+01"));
  PRINTF("got '%s'\r\n", test_buf);

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.e", 9.99) == 8);
  MU_TEST(check_test_emitter("   1e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.e", 9.99) == 8);
  MU_TEST(check_test_emitter("  1.e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.e", 9.99) == 8);
  MU_TEST(check_test_emitter("  +1e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.e", 9.99) == 8);
  MU_TEST(check_test_emitter("1e+01   "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.e", 9.99) == 8);
  MU_TEST(check_test_emitter("0001e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%8.1e", 9.99) == 8);
  MU_TEST(check_test_emitter(" 1.0e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "% 8.1e", 9.99) == 8);
  MU_TEST(check_test_emitter(" 1.0e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%#8.1e", 9.99) == 8);
  MU_TEST(check_test_emitter(" 1.0e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%+8.1e", 9.99) == 8);
  MU_TEST(check_test_emitter("+1.0e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.1e", 9.99) == 8);
  MU_TEST(check_test_emitter("1.0e+01 "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.1e", 9.99) == 8);
  MU_TEST(check_test_emitter("01.0e+01"));

  MU_TEST(mu_printf(test_emitter, NULL, "%-8.1E", 9.99) == 8);
  MU_TEST(check_test_emitter("1.0E+01 "));

  MU_TEST(mu_printf(test_emitter, NULL, "%08.1E", 9.99) == 8);
  MU_TEST(check_test_emitter("01.0E+01"));
}

void mu_printf_test() {
  PRINTF("begin tests...\r\n");
  mu_null_emitter_test();
  mu_precision_test();
  mu_pad_test();
  mu_floor_log10_test();
  mu_pow10_test();
  mu_puti_test();
  mu_putf_test();
  mu_parse_directive_test();
  mu_printf_c_test();
  mu_printf_s_test();
  mu_printf_d_test();
  mu_printf_u_test();
  // mu_printf_f_test();
  mu_printf_e_test();
  PRINTF("...end of tests\r\n");
}

#ifdef STANDALONE

int main() {
  mu_printf_test();
}

#endif
