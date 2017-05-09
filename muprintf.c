/*
 * gcc -Wall -DTEST_MUPRINTF muprintf.c -o muprintf && ./muprintf
 * muprintf.c
 *
 *  Created on: May 2, 2017
 *  Author: rdpoor@gmail.com
 */

#include "muprintf.h"
#include <stdarg.h>

#ifndef bool
typedef enum {false, true} bool;
#endif

// =============================================================================
// forward declarations

static char int_to_digit(int value, int base);
static int ipow(int base, int exp);

int mu_ftoa(putchar_fn_t putchar_fn,
                   void *obj,
                   float val,
                   int precision);
int mu_ftoa_aux(putchar_fn_t putchar_fn,
                void *obj,
                float val,
                int p10,
                bool carry);
int mu_ftosci(putchar_fn_t putchar_fn,
              void *obj,
              float val,
              int precision);
int mu_itoa(putchar_fn_t putchar_fn,
            void *obj,
            int i,
            int base);
int mu_itoa_aux(putchar_fn_t putchar_fn,
                void *obj,
                unsigned int i,
                int base);


// =============================================================================
// code

// Thank you http://stackoverflow.com/a/14997413/558639
static int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

static char int_to_digit(int value, int base) {
  int rem = positive_modulo(value, base);
  return (rem < 10) ? rem + '0' : (rem - 10) + 'a';
}

// Thank you http://stackoverflow.com/a/101613/558639
int ipow(int base, int exp) {
    int result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

/*!
 * @brief Print a floating point value with specified precision.
 *
 * @param putchar_fn Pointer to printer function
 * @param obj Object passed as first argument to putchar_fn
 * @param val Floating point value to be printed
 * @param precision Number of zeros following the decimal point.  Must
 *        be non-negative.  If zero, the decimal point is suppressed.
 * @returns The number of characters printed.
 */
int mu_ftoa(putchar_fn_t putchar_fn, void *obj, float val, int precision) {
  if (val < 0) {
    muputc(putchar_fn, obj, '-');
    return 1 + mu_ftoa(putchar_fn, obj, -val, precision);
  } else {
    float v = val * ipow(10, precision);
    // round up if the fractional remainder is > 0.5
    bool round_up = (val - (int)val) >= 0.5;
    return mu_ftoa_aux(putchar_fn, obj, v, -precision, round_up);
  }
}

// Implementation notes:
// Written as a postfix-recursive function to print digits out in order
// Handles rounding from lower order digits
// Initially called with val scaled by 10^precision; each call computes
//   the (next) least significant digit.
int mu_ftoa_aux(putchar_fn_t putchar_fn,
                void *obj,
                float val,
                int p10,        // power of ten being processed
                bool round_up) {
  unsigned int val_i = (unsigned int)val;
  if (val_i == 0 && p10 > 0) {
    // Endgame: print a final 1 if rounding carried over
    if (round_up) {
      muputc(putchar_fn, obj, '1');
      return 1;
    } else {
      return 0;
    }
  }
  // extract least significant digit at current power of 10
  int digit = (val_i + (round_up ? 1 : 0)) % 10;
  int n_printed = mu_ftoa_aux(putchar_fn,
                              obj,
                              val / 10,  // "shift right"
                              p10 + 1,   // next power of 10
                              // rounding in effect and digit rolled over?
                              round_up && (digit == 0));
  if (p10 == -1) {  // its time to print the decimal point
    muputc(putchar_fn, obj, '.');
    n_printed += 1;
  }
  muputc(putchar_fn, obj, digit + '0');
  return n_printed + 1;
}

/*!
 * @brief Print a floating point number in scientific format
 */
int mu_ftosci(putchar_fn_t putchar_fn, void *obj, float val, int precision) {
  int shift = 0;

  if (val < 0) {
    muputc(putchar_fn, obj, '-');
    return 1 + mu_ftosci(putchar_fn, obj, -val, precision);
  }
  
  if (val == 0.0) {
    shift = 0;
  } else {
    // normalize val
    while (val >= 10) {
      val = val / 10;
      shift += 1;
    }
    while (val < 1.0) {
      val = val * 10;
      shift -= 1;
    }
  }
  int n_printed = mu_ftoa(putchar_fn, obj, val, precision);
  muputc(putchar_fn, obj, 'e');
  if (shift >= 0) {
    muputc(putchar_fn, obj, '+');
    if (shift < 10) {
      muputc(putchar_fn, obj, '0');
    }
    mu_itoa(putchar_fn, obj, shift, 10);
  } else {
    muputc(putchar_fn, obj, '-');
    if (shift > -10) {
      muputc(putchar_fn, obj, '0');
    }
    mu_itoa(putchar_fn, obj, -shift, 10);
  }
  return n_printed + 4;
}
 
/*!
 * @brief Print an integer in the given base.
 * 
 * @param putchar_fn Pointer to printer function
 * @param obj Object passed as first argument to putchar_fn
 * @param i Integer to be printed
 * @param base Printing base.  Must be between 2 and 26 (incusive).
 */
int mu_itoa(putchar_fn_t putchar_fn, void *obj, int i, int base) {
  if (i == 0) {
    // handle zero case specially
    muputc(putchar_fn, obj, '0');
    return 1;

  } else if ((i < 0) && (base == 10)) {
    // handle negatives, but only for base 10
    muputc(putchar_fn, obj, '-');
    return mu_itoa_aux(putchar_fn, obj, -i, base) + 1;

  } else {
    // note that i will be treated as an unsigned in mu_itoa_aux
    return mu_itoa_aux(putchar_fn, obj, i, base);
  }    
}

int mu_itoa_aux(putchar_fn_t putchar_fn, void *obj, unsigned int i, int base) {
  if (i == 0) {
    return 0;
  } else {
    int n = mu_itoa_aux(putchar_fn, obj, i/base, base);
    muputc(putchar_fn, obj, int_to_digit(i, base));
    return n+1;
  }
}

// =============================================================================
// public methods
 
int muputc(putchar_fn_t putchar_fn, void *obj, char c) {
  putchar_fn(obj, c);
  return 1;
}

int muputs(putchar_fn_t putchar_fn, void *obj, char *c) {
  int n_chars = 0;
  while (*c) {
    putchar_fn(obj, *c++);
    n_chars += 1;
  }
  return n_chars;
}

int muprintf(putchar_fn_t putchar_fn, void *obj, const char *fmt_s, ...) {
  va_list ap;
  int result;

  va_start(ap, fmt_s);
  result = muprintf_internal(putchar_fn, obj, fmt_s, ap);
  va_end(ap);

  return result;
}

int muprintf_internal(putchar_fn_t putchar_fn,
                     void *obj,
                     char const *fmt,
                     va_list arg) {
  int int_temp;
  char char_temp;
  char *string_temp;
  double double_temp;

  char ch;
  int length = 0;

  while ( (ch = *fmt++) ) {
    if ( '%' == ch ) {
      switch (ch = *fmt++) {
      /* %% - print out a single %    */
      case '%':
        length += muputc(putchar_fn, obj, '%');
        break;

        /* %c: print out a character    */
      case 'c':
        char_temp = va_arg(arg, int);
        length += muputc(putchar_fn, obj, char_temp);
        break;

        /* %s: print out a string       */
      case 's':
        string_temp = va_arg(arg, char *);
        length += muputs(putchar_fn, obj, string_temp);
        break;

        /* %d: print out an int         */
      case 'd':
        int_temp = va_arg(arg, int);
        length += mu_itoa(putchar_fn, obj, int_temp, 10);
        break;

        /* %x: print out an int in hex  */
      case 'x':
        int_temp = va_arg(arg, int);
        length += mu_itoa(putchar_fn, obj, int_temp, 16);
        break;

      case 'f':
        double_temp = va_arg(arg, double);
        length += mu_ftoa(putchar_fn, obj, double_temp, 6);
        break;

      case 'e':
        double_temp = va_arg(arg, double);
        length += mu_ftosci(putchar_fn, obj, double_temp, 6);
        break;
      }
    }
    else {
      length += muputc(putchar_fn, obj, ch);
    }
  }
  return length;
}

// =============================================================================
#ifdef TEST_MUPRINTF
#include <stdio.h>
#include <string.h>

char g_str[80];
int g_index;


int my_putchar(void *obj, char ch) {
  return putchar(ch);
}

int test_putchar(void *obj, char ch) {
  g_str[g_index++] = ch;
  return 1;
}

// Minimal test suite.  Note that our goal is not a 100% reproduction
// of printf() behaviour, but something "close enough", so occasional
// discrepencies are acceptable.
// 
void test_muprintf_i(char *fmt, int val) {
  char expected_string[30];
  int expected_return = sprintf(expected_string, fmt, val);
  g_index = 0;
  int found_return = muprintf(test_putchar, NULL, fmt, val);
  g_str[g_index] = '\0';
  char *found_string = g_str;

  printf("%s, %d => ", fmt, val);
  if (strcmp(found_string, expected_string) != 0) {
    printf("expected %s, got %s\n", expected_string, found_string);
  } else if (expected_return != found_return) {
    printf("expected %d, got %d\n", expected_return, found_return);
  } else {
    printf("...okay\n");
  }
}
  
void test_muprintf_f(char *fmt, float val) {
  char expected_string[30];
  int expected_return = sprintf(expected_string, fmt, val);
  g_index = 0;
  int found_return = muprintf(test_putchar, NULL, fmt, val);
  g_str[g_index] = '\0';
  char *found_string = g_str;

  printf("%s, %f => ", fmt, val);
  if (strcmp(found_string, expected_string) != 0) {
    printf("expected %s, got %s\n", expected_string, found_string);
  } else if (expected_return != found_return) {
    printf("expected %d, got %d\n", expected_return, found_return);
  } else {
    printf("...okay\n");
  }
}
  
int main() {

  test_muprintf_i("%d", 0);
  test_muprintf_i("%d", 1);
  test_muprintf_i("%d", -1);

  test_muprintf_i("%x", 0);
  test_muprintf_i("%x", 1);
  test_muprintf_i("%x", -1);

  test_muprintf_f("%f", 0);
  test_muprintf_f("%f", 1);
  test_muprintf_f("%f", -1);
  test_muprintf_f("%f", 9.9999999);
  test_muprintf_f("%f", 0.1);
  test_muprintf_f("%f", -0.1);
  test_muprintf_f("%f", 10.0);
  test_muprintf_f("%f", -10.0);

  test_muprintf_f("%e", 0.0);
  test_muprintf_f("%e", 1.234e-10);
  test_muprintf_f("%e", 1.234e+10);
  test_muprintf_f("%e", -1.234e-10);
  test_muprintf_f("%e", -1.234e+10);
  
  return 0;
}

#endif
