/*
 * muemitf.h - a lightweight emitf for embedded systems
 *
 *  Created on: May 2, 2017
 *  Author: rdpoor@gmail.com
 */

#ifndef SOURCE_MUPRINTF_H_
#define SOURCE_MUPRINTF_H_

#include <stdarg.h>
#include <stdbool.h>

typedef int (*emitter_t)(void *obj, char ch);

/**
 * Controls padding when filling out to minimum width
 */
typedef enum { SPACE_LEFT, ZERO_LEFT, SPACE_RIGHT } mu_padding_t;
/*
 * These are technically internal routines, but exposed here primarily
 * for testability, and secondarily since they might be useful in some
 * contexts.
 */

/*!
 * @brief Null emitter function
 * This function simply returns 1.  It is useful for when you want to use
 * mprintf() to return the number of characters output.
 */
int mu_null_emitter(void *obj, const char c);

/*!
 * @brief Emit n pad characters if n is strictly positive.
 * @param emitter_fn Pointer to char emitter function
 * @param obj Pointer-sized user specified arg passed to emitter_fn
 * @param n The number of pad characters to emit.
 * @param c The pad character.
 * @return The number of characters emitted.
 */
int mu_pad(emitter_t emitter_fn, void *obj, int n, const char c);

/*!
 * @brief Return floor(log10(x)).
 *
 * Assumes x is strictly positive. Note: If n = mu_log10(x), then
 * 1.0 <= x*(10^-n) < 10.0
 *
 * @param x - The value applied to the function.
 * @return floor(log10(x))
 */
int mu_log10(float x);

/*!
 * @brief return 10^p
 *
 * Note that p may be negative.
 */
float mu_pow10(int p);

/*!
 * @brief Print a formatted integer
 *
 * Note that if base is 10, v is treated as a signed value, otherwise
 * unsigned.
 *
 * @param emitter_fn Pointer to char emitter function
 * @param obj Pointer-sized user specified arg passed to emitter_fn
 * @param v Integer value to emit
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

#endif /* SOURCE_MUPRINTF_H_ */
