/*
 * gcc -Wall -DTEST_MUPRINTF muprintf.c -o muprintf && ./muprintf
 * muprintf.c
 *
 *  Created on: May 2, 2017
 *  Author: rdpoor@gmail.com
 */

#include "mu_printf.h"
#include <stdarg.h>

// happy gnu extensions
#define MAX(a,b) ({ \
  __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a > _b ? _a : _b; \
})

#define MIN(a,b) ({ \
  __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a < _b ? _a : _b; \
})

// =============================================================================
// forward declarations

char const *parse_decimal(uint8_t *val, char const *str);
int process_directive(mu_directive_t *directive, va_list arg);
int process_c_directive(mu_directive_t *directive, unsigned int ch);
int process_d_directive(mu_directive_t *directive, int v);
int process_e_directive(mu_directive_t *directive, double v);
int process_f_directive(mu_directive_t *directive, double v);
int process_s_directive(mu_directive_t *directive, char const *str);
int process_u_directive(mu_directive_t *directive, unsigned int v, int base);
int emit_float_aux(emitter_t emitter, void *obj, float v, int p10, bool round_up);


// ======================================================================
// Code

int mu_null_emitter(void *obj, const char c) {
  return 1;
}

float mu_precision(float v, int ndigits) {
  if (v == 0.0) {
    return 0.0;
  } else if (v < 0.0) {
    return -mu_precision(-v, ndigits);
  }
  float p10 = mu_pow10(ndigits);
  float v1 = v * p10;
  float v2 = (int)(v1 + 0.5);
  float v3 = v2 / p10;
  return v3;
}

int mu_emit_char(emitter_t emitter_fn, void *obj, const char c) {
  emitter_fn(obj, c);
  return 1;
}

/*
 * Emit n pad chars, or none if n is zero or negative.  Returns the number of
 * chars emitted.
 */
int mu_emit_pad(emitter_t emitter_fn, void *obj, const char c, int n) {
  int i;

  for (i=0; i<n; i++) {
    mu_emit_char(emitter_fn, obj, c);
  }
  return i;
}

/*
 * Emit characters from str until a null is found or limit is reached, whichever
 * comes first.  A negative limit means no limit.
 */
int mu_emit_str(emitter_t emitter_fn, void *obj, const char *str, int limit) {
  char ch;
  int n_printed = 0;
  while((ch = *str++) && (limit != 0)) {
    n_printed += mu_emit_char(emitter_fn, obj, ch);
    limit -= 1;
  }
  return n_printed;
}

int mu_strlen(char const *str) {
  int len = 0;
  while (*str++) len++;
  return len;
}

int mu_floor_log10(float x) {
  int p = 0;
  while (x < 1.0) {
    x *= 10.0;
    p -= 1;
  }
  while (x >= 10.0) {
    x *= 0.1;
    p += 1;
  }
  return p;
}

// Return 10^p (note that p can be negative).
float mu_pow10(int p) {
  if (p < 0) {
    float r1 = 1.0 / mu_pow10(-p);
    return r1;
  }
  int x = 10;
  float res = 1.0;
  while (p > 0) {
    if (p & 1) {
      res = res * x;
    }
    x = x * x;
    p >>= 1;
  }
  return res;
}

char int_to_digit(unsigned int v, int base, bool upper) {
  int rem = v % base;
  if (rem < 10) {
    return rem + '0';
  } else if (upper) {
    return rem - 10 + 'A';
  } else {
    return rem - 10 + 'a';
  }
}

int mu_emit_integer(
    emitter_t emitter_fn,
    void *obj,
    unsigned int v,
    unsigned int base,
    bool upper) {
  if (v == 0) {
    return 0;
  }
  int n = mu_emit_integer(emitter_fn, obj, v / base, base, upper);
  mu_emit_char(emitter_fn, obj, int_to_digit(v, base, upper));
  return n + 1;
}

int mu_emit_float(emitter_t emitter,
                  void *obj,
                  float v,
                  unsigned int precision) {
  // preamble: decide if lowest order digit is subject to rounding
  float scaled_v = v * mu_pow10(precision);
  bool round_up = (scaled_v - (int)scaled_v) >= 0.5;
  return emit_float_aux(emitter, obj, v, -precision, round_up);
}

// emit the digit in the 10^pow10 position after emitting higher-order digits
int emit_float_aux(emitter_t emitter, void *obj, float v, int pow10, bool round_up) {
  unsigned int scaled_vi = v * mu_pow10(-pow10);
  if (scaled_vi == 0 && pow10 > 0) {
    // Endgame: emit a final 1 if rounding carried over
    if (round_up) {
      mu_emit_char(emitter, obj, '1');
      return 1;
    } else {
      return 0;
    }
  }
  // extract least significant digit at current power of 10
  int digit = (scaled_vi + (round_up ? 1 : 0)) % 10;
  // emit higher order digits
  int n = emit_float_aux(emitter, obj, v, pow10 + 1, round_up && (digit == 0));
  // is it time to print the decimal point?
  if (pow10 == -1) {
    mu_emit_char(emitter, obj,'.');
    n += 1;
  }
  mu_emit_char(emitter, obj, digit + '0');
  return n + 1;
}

int mu_printf(emitter_t emitter_fn, void *obj, const char *fmt_s, ...) {
  va_list ap;
  int result;

  va_start(ap, fmt_s);
  result = mu_vprintf(emitter_fn, obj, fmt_s, ap);
  va_end(ap);

  return result;
}

int mu_vprintf(emitter_t emitter_fn, void *obj, char const *fmt, va_list args) {
  char ch;
  mu_directive_t directive;
  int n_printed = 0;

  directive.emitter_fn = emitter_fn;
  directive.emitter_arg = obj;

  // toplevel
  while ((ch = *fmt++)) {
    if ('%' == ch) {
      fmt = mu_parse_directive(&directive, fmt);
      n_printed += process_directive(&directive, args);
    } else {
      mu_emit_char(emitter_fn, obj, ch);  // process ordinary character
      n_printed += 1;
    }
  }
  return n_printed;
}

char const *mu_parse_directive(mu_directive_t *directive, char const *fmt) {
  char ch;

  directive->flags.all = 0;
  directive->width = 0;
  directive->precision = MU_PRECISION_NOT_GIVEN;

  // parse #0- + flags...
  bool parsing_flags = true;
  while(parsing_flags) {
    ch = *fmt;
    switch(ch) {
    case '#':
      directive->flags.alternate_form = 1;
      break;
    case '0':
      directive->flags.pad_zero = 1;
      break;
    case '-':
      directive->flags.pad_right = 1;
      break;
    case ' ':
      directive->flags.pad_space = 1;
      break;
    case '+':
      directive->flags.pad_plus = 1;
      break;
    default:
      parsing_flags = false;
    }
    if (parsing_flags) {
      fmt++;
    }
  }
  // fix mutual exclusions
  if (directive->flags.pad_right) directive->flags.pad_zero = 0;
  if (directive->flags.pad_plus) directive->flags.pad_space = 0;

  // parse field width
  fmt = parse_decimal(&directive->width, fmt);

  // parse precision
  if (*fmt == '.') {
    fmt++;
    fmt = parse_decimal(&directive->precision, fmt);
  }

  // parse conversion char
  ch = *fmt++;
  if (ch >= 'A' && ch <= 'Z') {
    directive->flags.upper_case = 1;
    ch = ch - 'A' + 'a';  // convert to lower case
  }
  directive->conversion = ch;

  return fmt;
}

// =============================================================================
// =============================================================================

/*!
 * Parse an unsigned decimal string, breaking on first non-digit.  Return value
 * by reference, return pointer to first non-digit.
 */
char const *parse_decimal(uint8_t *val, char const *str) {
  char ch;
  uint8_t v = 0;
  while(true) {
    ch = *str;
    if (ch < '0' || ch > '9') break;
    v = (v * 10) + (ch - '0');
    str++;
  }
  *val = v;
  return str;
}



int process_directive(mu_directive_t *directive, va_list arg) {
  switch(directive->conversion) {
  case '%':
    return process_c_directive(directive, '%');

  case 'b':
    return process_u_directive(directive, va_arg(arg, unsigned int), 2);

  case 'c':
    return process_c_directive(directive, va_arg(arg, unsigned int));

  case 'd':
  case 'i':
    return process_d_directive(directive, va_arg(arg, int));

  case 'E':
  case 'e':
    return process_e_directive(directive, va_arg(arg, double));

  case 'F':
  case 'f':
    return process_f_directive(directive, va_arg(arg, double));

  case 'o':
    return process_u_directive(directive, va_arg(arg, unsigned int), 8);

  case 's':
    return process_s_directive(directive, va_arg(arg, char const *));

  case 'p':
    directive->flags.alternate_form = true;
    // fall through ---vvv
  case 'X':
  case 'x':
    return process_u_directive(directive, va_arg(arg, unsigned int), 16);

  default:
    return 0;
  }
}

/*
 * Process a character.  Flags are ignored.
 */
int process_c_directive(mu_directive_t *directive, unsigned int ch) {
  return mu_emit_char(directive->emitter_fn, directive->emitter_arg, ch);
}

int process_diox_directive(mu_directive_t *directive,
                           unsigned int v,
                           bool is_negative,
                           int base) {
  unsigned int n_significant;  // # of significant digits in v
  unsigned int n_required;     // # of digits that must be printed

  // how many digits will be printed?
  n_significant = mu_emit_integer(mu_null_emitter, (void *)0, v, base, false);
  if (directive->precision == MU_PRECISION_NOT_GIVEN) {
    n_required = MAX(1, n_significant);
  } else {
    directive->flags.pad_zero = 0;
    n_required = MAX(directive->precision, n_significant);
  }

  // what are we printing just before the digits?
  char *prefix = "";
  char n_extra = 0;
  if (is_negative) {
    prefix = "-";
    n_extra = 1;
  } else if (n_required == 0) {
    // precision = 0 and value = 0 -- no prefix
  } else if (directive->flags.pad_plus) {
    prefix = "+";
    n_extra = 1;
  } else if (directive->flags.pad_space) {
    prefix = " ";
    n_extra = 1;
  } else if ((directive->flags.alternate_form) && (v != 0)) {
    if (base == 2) {
      prefix = (directive->flags.upper_case ? "0B" : "0b");
      n_extra = 2;
    } else if (base == 8) {
      prefix = "0";
      n_extra = 1;
    } else if (base == 16) {
      prefix = (directive->flags.upper_case ? "0X" : "0x");
      n_extra = 2;
    }
  }

  // how many pad characters will we print?
  int padding = directive->width - n_required - n_extra;

  // now output:
  int n_emitted = 0;
  // ...leading spaces
  if (!directive->flags.pad_right && !directive->flags.pad_zero) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             ' ',
                             padding);
  }
  // ...prefix
  n_emitted += mu_emit_str(directive->emitter_fn,
                           directive->emitter_arg,
                           prefix,
                           -1);
  // ...zero padding
  if (directive->flags.pad_zero) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             '0',
                             padding);
  }
  // ...leading zeros
  n_emitted += mu_emit_pad(directive->emitter_fn,
                           directive->emitter_arg,
                           '0',
                           n_required - n_significant);
  // ... the value itself
  n_emitted += mu_emit_integer(directive->emitter_fn,
                       directive->emitter_arg,
                       v,
                       base,
                       directive->flags.upper_case);

  if (directive->flags.pad_right) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             ' ',
                             padding);
  }

  return n_emitted;
}

/*
 * Process a signed decimal integer.
 */
int process_d_directive(mu_directive_t *directive, int v) {
  bool is_negative = v < 0;
  directive->flags.alternate_form = 0;  // %d doesn't honor %#d
  return process_diox_directive(directive,
                                is_negative ? -v : v,
                                is_negative,
                                10);
}

/*
 * Process a float using n.nnne+xx format
 */
int process_e_directive(mu_directive_t *directive, double v) {
  bool mantissa_is_neg = v < 0;
  if (mantissa_is_neg) v = -v;
  unsigned int mantissa_width;
  unsigned int exponent_width;
  int exponent;
  bool exponent_is_neg = false;

  if (directive->precision == MU_PRECISION_NOT_GIVEN) {
    directive->precision = 6;
  }

  exponent = 0;
  if (v != 0.0) {
    // normalize v
    while (v >= 10) {
      v = v / 10;
      exponent += 1;
    }
    while (v < 1.0) {
      v = v * 10;
      exponent -= 1;
    }
  }
  // at this point, 1.0 <= v < 10 (unless v is zero), and exponent is the power
  // of 10 that will restore it to its original value.

  // how wide is the mantissa?
  mantissa_width = 1 + directive->precision;
  // printing the decimal point?
  if (directive->precision > 0 || directive->flags.alternate_form) {
    mantissa_width += 1;
  }
  // how many sigificant digits in exponent?
  if (exponent < 0) {
    exponent = -exponent;
    exponent_is_neg = true;
  }
  exponent_width = mu_emit_integer(mu_null_emitter,
                                   (void *) 0,
                                   exponent,
                                   10,
                                   false);
  // what prefix gets printed just before the mantissa?
  char *prefix = "";
  char n_extra = 0;
  if (mantissa_is_neg) {
    prefix = "-";
    n_extra = 1;
  } else if (directive->flags.pad_plus) {
    prefix = "+";
    n_extra = 1;
  } else if (directive->flags.pad_space) {
    prefix = " ";
    n_extra = 1;
  }

  // how many pad characters will we print?
  int padding = directive->width - n_extra - mantissa_width - 2 -
      MAX(2, exponent_width);

  // now output:
  int n_emitted = 0;
  // ...leading spaces
  if (!directive->flags.pad_right && !directive->flags.pad_zero) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             ' ',
                             padding);
  }
  // ...prefix
  n_emitted += mu_emit_str(directive->emitter_fn,
                           directive->emitter_arg,
                           prefix,
                           -1);
  // ...zero padding
  if (directive->flags.pad_zero) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             '0',
                             padding);
  }
  // ...the mantissa
  n_emitted += mu_emit_float(directive->emitter_fn,
                       directive->emitter_arg,
                       v,
                       directive->precision);
  // ...any explicit trailing '.'
  if (directive->precision == 0 && directive->flags.alternate_form) {
    n_emitted += mu_emit_char(directive->emitter_fn,
                              directive->emitter_arg,
                              '.');
  }
  // ...the exponent
  n_emitted += mu_emit_char(directive->emitter_fn,
                            directive->emitter_arg,
                            directive->flags.upper_case ? 'E' : 'e');
  n_emitted += mu_emit_char(directive->emitter_fn,
                            directive->emitter_arg,
                            exponent_is_neg ? '-' : '+');
  n_emitted += mu_emit_pad(directive->emitter_fn,
                           directive->emitter_arg,
                           '0',
                           2 - exponent_width);
  n_emitted += mu_emit_integer(mu_null_emitter,
                               (void *) 0,
                               exponent,
                               10,
                               false);
  // ...trailing padding
  if (directive->flags.pad_right) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             ' ',
                             padding);
  }

  return n_emitted;
}

/*
 * Process a float
 */
int process_f_directive(mu_directive_t *directive, double v) {
  bool is_negative = v < 0;
  if (is_negative) v = -v;
  unsigned int n_characters;  // # of chars in atof(v)

  if (directive->precision == MU_PRECISION_NOT_GIVEN) {
    directive->precision = 6;
  }
  // how many digits will be printed?
  n_characters = mu_emit_float(mu_null_emitter, (void *)0, v, directive->precision);

  // what are we printing just before the digits?
  char *prefix = "";
  char n_extra = 0;
  if (is_negative) {
    prefix = "-";
    n_extra = 1;
  } else if (directive->flags.pad_plus) {
    prefix = "+";
    n_extra = 1;
  } else if (directive->flags.pad_space) {
    prefix = " ";
    n_extra = 1;
  }

  // are we going to print an explicit trailing '.'?
  if (directive->precision == 0 && directive->flags.alternate_form) {
    n_extra += 1;
  }

  // how many pad characters will we print?
  int padding = directive->width - n_characters - n_extra;

  // now output:
  int n_emitted = 0;
  // ...leading spaces
  if (!directive->flags.pad_right && !directive->flags.pad_zero) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             ' ',
                             padding);
  }
  // ...prefix
  n_emitted += mu_emit_str(directive->emitter_fn,
                           directive->emitter_arg,
                           prefix,
                           -1);
  // ...zero padding
  if (directive->flags.pad_zero) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             '0',
                             padding);
  }
  // ... the value itself
  n_emitted += mu_emit_float(directive->emitter_fn,
                       directive->emitter_arg,
                       v,
                       directive->precision);
  // ... explicit trailing '.'
  if (directive->precision == 0 && directive->flags.alternate_form) {
    n_emitted += mu_emit_char(directive->emitter_fn,
                              directive->emitter_arg,
                              '.');
  }
  // ... trailing padding
  if (directive->flags.pad_right) {
    n_emitted += mu_emit_pad(directive->emitter_fn,
                             directive->emitter_arg,
                             ' ',
                             padding);
  }

  return n_emitted;

}

/*
 * Process an unsigned integer in one of several bases...
 */
int process_u_directive(mu_directive_t *directive, unsigned int v, int base) {
  return process_diox_directive(directive, v, false, base);
}

int process_s_directive(mu_directive_t *directive, char const *str) {
  // how many characters in str are we going to print?
  int slimit = mu_strlen(str);
  if ((directive->precision != MU_PRECISION_NOT_GIVEN) &&
      (directive->precision < slimit)) {
    slimit = directive->precision;
  }
  // how many pad chars are we going to print?
  int padding = directive->width - slimit;

  int n_emitted = 0;

  if (directive->flags.pad_right) {
    // output string then padding
    n_emitted += mu_emit_str(directive->emitter_fn,
                             directive->emitter_arg,
                             str,
                             slimit);
  }
  n_emitted += mu_emit_pad(directive->emitter_fn,
                           directive->emitter_arg,
                           ' ',
                           padding);
  if (!directive->flags.pad_right) {
    // output padding then string
    n_emitted += mu_emit_str(directive->emitter_fn,
                             directive->emitter_arg,
                             str,
                             slimit);
  }
  return n_emitted;
}

