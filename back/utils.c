/**
 * Copyright 2018 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "utils.h"
#include <stdarg.h>
#include <stdio.h>

const char * utils_file_name = "<stdin>";

const char * get_utils_file_name()
{
    return utils_file_name;
}

void set_utils_file_name(const char * file_name)
{
    utils_file_name = file_name;
}

void print_error_msg(int line_no, const char * format, ...)
{
    va_list args;

    va_start(args, format);

    fprintf(stderr, "%s:%d: error: ", utils_file_name, line_no);
    vfprintf(stderr, format, args);

#ifndef NO_FFI
    fprintf(stderr, "\n");
#else
    fprintf(stderr, "\r\n");
#endif

    va_end(args);
}

void print_warning_msg(int line_no, const char * format, ...)
{
    va_list args;

    va_start(args, format);

    fprintf(stderr,"%s:%d: warning: ", utils_file_name, line_no);
    vfprintf(stderr, format, args);

#ifndef NO_FFI
    fprintf(stderr, "\n");
#else
    fprintf(stderr, "\r\n");
#endif

    va_end(args);
}
