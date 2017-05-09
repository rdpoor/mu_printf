# mu_printf: a lightweight printf() for embedded systems

Do you want to print floating point numbers in your embedded application
without dragging in stdio, math and other libraries?

Do you want to control printing to multiple serial ports, string buffers and log
files wthout frustration?

mu_printf() may be just what you're looking for.

## key features

mu_printf() supports the following familiar formats:

* %% quote a literal %
* %c print a single character
* %d print an integer in decimal format
* %e print a float in scientific format
* %f print a float with six digits of precision
* %s print a string
* %x print an integer in hexadecimal format

mu_printf() handles rounding of floating point values, so that

    "%f", 9.999999 => "10.000000"

Hexadecimal formats are treated as unsigned:

    "%x", -1 => "ffffffff"

## Limitations

mu_printf() does not support any of the `flag`, `width`, `precision` or `length`
modifiers of conventional printf.

## Controlling output stream

Rather than providing the myriad of printf variants (printf, fprintf, sprintf,
snprintf), mu_printf() takes two addtional arguments:

    int muprintf(putchar_fn_t putchar_fn, void *obj, const char *fmt_s, ...);

`putchar_fn` is a pointer to a function whose contract is to output a single
character on the desired stream.  `obj` is a user-supplied object passed to
the putchar_fn.  This allows for great flexibility in where characters are
printed.  Some examples:

### Print to stdout

    void stdout_putchar(void *obj, char ch) {
      putchar(ch);
    }
    int printf(const char *fmt_s, ...) {
      va_list ap;
      int result;

      va_start(ap, fmt_s);
      result = muprintf_internal(stdout_putchar, NULL, fmt_s, ap);
      va_end(ap);

      return result;
    }

### Print to a string buffer

    char g_buffer[80];
    int g_index;

    void sprintf_start() {
      g_index = 0;
    }

    void sprintf_end() {
      g_buffer[g_index] = '\0';
    }
      
    void sprintf_putchar(void *obj, char ch) {
      g_buffer[g_index++] = ch;
    }

    int sprintf(const char *fmt_s, ...) {
      va_list ap;
      int result;

      sprintf_start();
      va_start(ap, fmt_s);
      result = muprintf_internal(sprintf_putchar, NULL, fmt_s, ap);
      va_end(ap);
      sprintf_end();
      
      return result;
    }

 ## API

    /*
     * the muprintf() family of functions each take a function pointer as
     * the first argument, which has the signature:
     *
     *   int my_putchar(void *obj, char ch)
     *
     * where
     *   obj is a pointer-sized object of your choice
     *   ch is the character to be printed / stored / whatever
     */
    typedef int (*putchar_fn_t)(void *obj, char ch);

    /*
     * Output a single character.  Returns 1.
     */
    int muputc(putchar_fn_t putchar_fn, void *obj, char c);

    /*
     * Output a null-terminated string.  Returns the number of characters
     * written (excluding the null termination).
     */
    int muputs(putchar_fn_t putchar_fn, void *obj, char *c);

    /*
     * Output a formatted string.  Returns the number of characters written.
     * The following conversion specifiers are honored:
     *
     * %% quote a literal %
     * %c print a single character
     * %d print an integer in decimal format
     * %e print a float in scientific format
     * %f print a float with six digits of precision
     * %s print a string
     * %x print an integer in hexadecimal format
     */
    int muprintf(putchar_fn_t putchar_fn, void *obj, const char *fmt_s, ...);

    /*
     * muprintf_internal() is identical to muprintf(), but assumes that the
     * varargs have already been parsed.
     */
    int muprintf_internal(putchar_fn_t putchar_fn,
                         void *obj,
                         char const *fmt,
                         va_list arg);

