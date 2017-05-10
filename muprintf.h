/*
 * muprintf.h - a lightweight printf for embedded systems
 *
 *  Created on: May 2, 2017
 *  Author: rdpoor@gmail.com
 */

#ifndef SOURCE_MUPRINTF_H_
#define SOURCE_MUPRINTF_H_

#include <stdarg.h>

typedef int (*putchar_fn_t)(void *obj, char ch);

/*!
 * The following methods take a function pointer as the first argument, which
 * has the signature:
 *    int my_putchar(void *obj, char ch)
 * where 
 *    obj is a pointer-sized object of your choice
 *    ch is the character to be stored / printed / whatever
 * my_putchar() should return a value >= 0 on success, < 0 on an error.
 */
int muputc(putchar_fn_t putchar_fn, void *obj, char c);
int muputs(putchar_fn_t putchar_fn, void *obj, char *c);
int muprintf(putchar_fn_t putchar_fn, void *obj, const char *fmt_s, ...);

// same as muprintf, but with parsed args.
int muprintf_internal(putchar_fn_t putchar_fn,
                     void *obj,
                     char const *fmt,
                     va_list arg);

#endif /* SOURCE_MUPRINTF_H_ */
