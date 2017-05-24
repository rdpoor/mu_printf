/*
 * gcc -Wall -DTEST_MUPRINTF muprintf.c -o muprintf && ./muprintf
 * muprintf.c
 *
 *  Created on: May 2, 2017
 *  Author: rdpoor@gmail.com
 */

#include <mu_printf.h>
#include <stdarg.h>
#include <stdint.h>

#ifndef bool
typedef enum {false, true} bool;
#endif

typedef union {
  uint8_t flags;
  struct {
    int upper_case:1;         // use E or X
    int alternate_form:1;     // '#' alternate form
    int pad_zero:1;           // '0' pad left with zeros
    int pad_right:1;          // '-' right padding
    int pad_positive_space:1; // ' ' add leading space on positive numeric
    int pad_positive_plus:1;  // '+' include + or -
  } __attribute__((__packed__));
} flags_t;

// =============================================================================
// forward declarations

// ======================================================================
// Code

int mu_null_emitter(void *obj, const char c) {
  return 1;
}

int mu_pad(emitter_t emitter_fn, void *obj, int n, const char c) {
  int i;

  for (i=0; i<n; i++) {
    emitter_fn(obj, c);
  }
  return i;
}

int mu_log10(float x) {
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

// Return 10^p (note that exp can be negative).
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


int mu_puti(
    emitter_t emitter_fn,
    void *obj,
    unsigned int v,
    unsigned int base,
    bool upper) {
  // ============================== lexically scoped
  char int_to_digit(int v) {
    int rem = v % base;
    if (rem < 10) {
      return rem + '0';
    } else if (upper) {
      return rem - 10 + 'A';
    } else {
      return rem - 10 + 'a';
    }
  }
  int mu_puti_aux(unsigned int v) {
    if (v == 0) {
      return 0;
    }
    int n = mu_puti_aux(v / base);
    emitter_fn(obj, int_to_digit(v));
    return n + 1;
  }
  // ============================== top level
  if (v == 0) {
    emitter_fn(obj, '0');
    return 1;
  } else {
    return mu_puti_aux(v);
  }
}

int mu_putf(
    emitter_t emitter_fn,
    void *obj,
    float v,
    unsigned int precision) {
  // ============================== lexically scoped
  // emit the digit in the 10^pow10 position after emitting higher-order digits
  int mu_putf_aux(int pow10, bool carry) {
    unsigned int scaled_vi = v * mu_pow10(-pow10);
    if (scaled_vi == 0 && pow10 > 0) {
      // Endgame: emit a final 1 if rounding carried over
      if (carry) {
        emitter_fn(obj, '1');
        return 1;
      } else {
        return 0;
      }
    }
    // extract least significant digit at current power of 10
    int digit = (scaled_vi + (carry ? 1 : 0)) % 10;
    // emit higher order digits
    int n = mu_putf_aux(pow10 + 1, carry && (digit == 0));
    if (pow10 == -1) {  // time to print the decimal point?
      emitter_fn(obj,'.');
      n += 1;
    }
    emitter_fn(obj, digit + '0');
    return n + 1;
  }
  // ============================== top level
  // decide if lowest order digit is subject to carry
  float scaled_v = v * mu_pow10(precision);
  int scaled_vi = scaled_v;
  float rem = scaled_v - scaled_vi;
  bool carry = (rem) >= 0.5;
  return mu_putf_aux(-precision, carry);
}

int mu_printf(emitter_t emitter_fn, void *obj, const char *fmt_s, ...) {
  va_list ap;
  int result;

  va_start(ap, fmt_s);
  result = mu_vprintf(emitter_fn, obj, fmt_s, ap);
  va_end(ap);

  return result;
}

int mu_vprintf(emitter_t emitter_fn, void *obj, char const *fmt, va_list arg) {
  char ch;
  int length = 0;
  flags_t flags;
  int field_width = 0;
  int precision = 0;

  // ==========================
  // internal functions (gcc extension)

  // extract flags between '%' and the conversion type char.  Returns with
  // flags structure filled in and fmt pointing to conversion type char.
  void parse_flags() {
    flags.flags = 0;  // zero out flags structure
    bool looking_at_flag = true;
    while(true) {
      ch = *fmt;
      switch(ch) {
      case '#':
        flags.alternate_form = 1;
        break;
      case '0':
        flags.pad_zero = 1;
        break;
      case '-':
        flags.pad_right = 1;
        break;
      case ' ':
        flags.pad_positive_space = 1;
        break;
      case '+':
        flags.pad_positive_plus = 1;
        break;
      default:
        looking_at_flag = false;
      }
      if (looking_at_flag) {
        fmt++;
      } else {
        break;
      }
    }
    // fix mutual exclusions
    if (flags.pad_right) flags.pad_zero = 0;
    if (flags.pad_positive_plus) flags.pad_positive_space = 0;
  }

  // parse an ascii string as an unsigned decimal integer.  return with
  // fmt pointing at first non-digit.
  int parse_decimal() {
    int val = 0;
    while(true) {
      ch = *fmt;
      if (ch < '0' || ch > '9') {
        break;
      }
      val *= 10;
      val += ch - '0';
      fmt++;
    }
    return val;
  }

  // parse the field width specifier.  return with field_width set to the
  // field width and fmt pointing at first non-field width char
  void parse_field_width() {
    field_width = parse_decimal();
  }

  // parse the precision specifier.  lead-in char is a period, followed by
  // decimal digits.  returns with fmt pointing at first non-digit and
  // precision set to given precision or -1 if not set.
  void parse_precision() {
    precision = -1;
    ch = *fmt;
    if (ch != '.') {
      return;
    }
    fmt++;
    precision = parse_decimal();
  }

  int strlen(char const *arg) {
    int len = 0;
    while (*arg++) len++;
    return len;
  }

  int emit_char(char ch) {
    emitter_fn(obj, ch);
    return 1;
  }

  int emit_str(char const *arg, int limit) {
    char ch;
    int len = 0;
    while((ch = *arg++) && (limit > 0)) {
      len += emit_char(ch);
      limit--;
    }
    return len;
  }

  int emit_pad(char const pad, int n) {
    if (n <= 0) {
      return 0;
    }
    for(int i=0; i<n; i++) {
      emit_char(pad);
    }
    return n;
  }

  int process_s_directive(char *arg) {
    int padding = 0;
    int arg_len = strlen(arg);
    if ((precision >= 0) && (precision < arg_len)) {
      // if precision given, limit string length
      arg_len = precision;
    }
    if (field_width > arg_len) {
      // if field_width given, pad resulting output
      padding = field_width - arg_len;
    }
    if (flags.pad_right) {
      emit_str(arg, arg_len);
      emit_pad(' ', padding);
    } else {
      emit_pad(' ', padding);
      emit_str(arg, arg_len);
    }
    return arg_len + padding;
  }

  int process_d_directive(int val) {
    int len = 0;
    unsigned int absval = (val >= 0) ? val : -val;

    // how many digits will be printed?
    int absval_len = 1;
    if (absval > 0) {
      absval_len = mu_puti(mu_null_emitter, (void *)0, absval, 10, false);
    }

    // if precision is given, it effectively turns off zero padding.
    if (precision >= 0) {
      flags.pad_zero = 0;
    }

    // how many padding zeros will be printed?
    int zero_padding = 0;
    if (precision > absval_len) {
      zero_padding = precision - absval_len;
    }

    // are we leaving space for leading '-', '+' or ' '?
    int extras_len = 0;
    if ((val < 0) || flags.pad_positive_space || flags.pad_positive_plus) {
      extras_len = 1;
    }

    // compute padding
    int space_padding = field_width - absval_len - zero_padding - extras_len;

    // now output...
    if (!flags.pad_right && !flags.pad_zero) {
      len += emit_pad(' ', space_padding);
    }

    if (val < 0) {
      len += emit_char('-');
    } else if (flags.pad_positive_plus) {
      len += emit_char('+');
    } else if (flags.pad_positive_space) {
      len += emit_char(' ');
    }

    if (!flags.pad_right && flags.pad_zero) {
      len += emit_pad('0', space_padding);
    }

    len += emit_pad('0', zero_padding);

    if (val == 0) {
      len += emit_char('0');
    } else {
      len += mu_puti(emitter_fn, obj, absval, 10, false);
    }

    if (flags.pad_right) {
      len += emit_pad(' ', space_padding);
    }

    return len;
  }

  int process_directive() {
    parse_flags();
    parse_field_width();
    parse_precision();
    ch = *fmt++;
    switch(ch) {
    case '%':
      emitter_fn(obj, '%');
      return 1;

    case 'c':
      emitter_fn(obj, va_arg(arg, unsigned int));
      return 1;

    case 'd':
    case 'i':
      return process_d_directive(va_arg(arg, int));

    case 'E':
      flags.upper_case = true;
      // vv -- fall through -- vv
    case 'e':
      break;
    case 'f':
      break;
    case 'o':
      break;
    case 'p':
      break;
    case 's':
      return process_s_directive(va_arg(arg, char *));

    case 'X':
      flags.upper_case = true;
      // vv -- fall through -- vv
    case 'x':
      break;
    default:
      break;
    }
    return 0;
  }

  // ==========================
  // toplevel
  while ((ch = *fmt++)) {
    if ('%' == ch) {
      length += process_directive();  // process % directive
    } else {
      emitter_fn(obj, ch);  // process ordinary character
      length += 1;
    }
  }
  return length;
}

