/*
 * mu_printf - a lightweight printf() for embedded systems
 *
 *  Created on: May 2, 2017
 *  Author: rdpoor@gmail.com
 */

#ifndef SOURCE_MU_PRINTF_H_
#define SOURCE_MU_PRINTF_H_

#include <stdarg.h>
#include <stdint.h>

#ifndef bool
typedef enum {false, true} bool;
#endif

/*!
 * @brief Template for "emit one char" method
 */
typedef int (*emitter_t)(void *obj, char ch);

typedef union {
  uint8_t all;             // all flag bits at once
  struct {
    int upper_case:1;      // use E or X
    int alternate_form:1;  // '#' alternate form
    int pad_zero:1;        // '0' pad left with zeros
    int pad_right:1;       // '-' right padding
    int pad_space:1;       // ' ' add leading space on positive numeric
    int pad_plus:1;        // '+' include + or -
  } __attribute__((__packed__));
} flags_t;

#define MU_PRECISION_NOT_GIVEN 255

/*!
 * To reduce the size of calling stack frames, we stuff state into a single
 * structure and pass that around.
 */
typedef struct {
  emitter_t emitter_fn;  // function that prints a char
  void *emitter_arg;     // user-supplied argument to emitter
  flags_t flags;
  uint8_t width;         // minimum width of resulting field
  uint8_t precision;     // %s: # of char to print, %f, %e: # digits after .
  char conversion;
} mu_directive_t;

/*!
 * @brief Null emitter function
 * This function simply returns 1.  It is useful for when you want to use
 * mu_printf() to compute the number of characters that will be output.
 */
int mu_null_emitter(void *obj, const char c);

/*!
 * @brief Emit one character
 */
int mu_emit_char(emitter_t emitter_fn, void *obj, const char c);

/*
 * These are technically internal routines, but exposed here primarily
 * for testability, and secondarily since they might be useful in some
 * contexts.
 */

/*!
 * @brief Emit n pad characters.
 *
 * Emit n pad characters.  If n is zero or negative, nothing is emitted.
 * Ultimately returns the number of characters emitted.
 *
 * @param emitter_fn Pointer to char emitter function
 * @param obj Pointer-sized user specified arg passed to emitter_fn
 * @param c The pad character.
 * @param n The number of pad characters to emit.
 * @return The number of characters emitted.
 */
int mu_emit_pad(emitter_t emitter_fn, void *obj, const char c, int n);

/*!
 * @brief Return floor(log10(x)).
 *
 * Assumes x is strictly positive. Note: If n = mu_floor_log10(x), then
 * 1.0 <= x*(10^-n) < 10.0
 *
 * @param x - The value applied to the function.
 * @return floor(log10(x))
 */
int mu_floor_log10(float x);

/*!
 * @brief return 10^p
 *
 * Note that p may be negative.
 */
float mu_pow10(int p);

/*!
 * @brief Print an unsigned integer.
 *
 * @param emitter_fn Pointer to char emitter function
 * @param obj Pointer-sized user specified arg passed to emitter_fn
 * @param v Integer value to emit. Assumed non-negative.
 * @param base Base in which to emit the integer.
 * @param upper When true, emit alpha chars in upper case.
 * @return Number of chars emitted.
 */
int mu_puti(
    emitter_t emitter_fn,
    void *obj,
    unsigned int v,
    unsigned int base,
    bool upper);

/*!
 * @brief Print a formatted float
 *
 * Note: when precision is 0, the decimal point is suppressed.
 *
 * @param emitter_fn Pointer to char emitter function
 * @param obj Pointer-sized user specified arg passed to emitter_fn
 * @param v Integer value to emit.  Assumed to be non-negative.
 * @param precision Number of digits to emit after decimal point.
 * @return Number of chars emitted.
 */
int mu_putf(
    emitter_t emitter_fn,
    void *obj,
    float v,
    unsigned int precision);

int mu_printf(emitter_t emitter_fn, void *obj, const char *fmt_s, ...);

/*!
 * @brief Identical to mu_printf(), but with pre-parsed arg list.
 */
int mu_vprintf(emitter_t emitter_fn, void *obj, char const *fmt, va_list arg);

/*!
 * Extract the parameters of a %...<c> directive.  Returns pointer to the
 * first char following the directive.
 */
char const *mu_parse_directive(mu_directive_t *directive, char const *fmt);

#endif /* SOURCE_MU_PRINTF_H_ */
