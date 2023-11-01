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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define MAX_MSG_SIZE 1024

static const char * utils_file_name = "<stdin>";
static unsigned int * utils_msg_count = NULL;
static unsigned int * utils_msg_array_size = NULL;
static char *** utils_msg_array = NULL;

const char * get_utils_file_name()
{
    return utils_file_name;
}

void set_utils_file_name(const char * file_name)
{
    utils_file_name = file_name;
}

void set_msg_buffer(unsigned int * msg_count, unsigned int * msg_array_size, char *** msg_array)
{
    utils_msg_count = msg_count;
    utils_msg_array_size = msg_array_size;
    utils_msg_array = msg_array;
}

void print_msg_buffer(unsigned int msg_count, char ** msg_array)
{
    if (msg_array == NULL)
    {
        return;
    }
    unsigned int i = 0;
    for (i = 0; i < msg_count; i++)
    {
        printf("%s\n", msg_array[i]);
    }
}

static void print_msg(const char * type, int line_no, const char * format, va_list args)
{
    va_list args_copy;

    va_copy(args_copy, args);
    fprintf(stderr, "%s:%d: %s: ", utils_file_name, line_no, type);
    vfprintf(stderr, format, args_copy);
    va_end(args_copy);

#ifndef NO_FFI
    fprintf(stderr, "\n");
#else
    fprintf(stderr, "\r\n");
#endif

    if (utils_msg_count != NULL)
    {
        unsigned int msg_len = 0;
        char msg_buf[MAX_MSG_SIZE] = { 0 };

        if (*utils_msg_count >= *utils_msg_array_size)
        {
            *utils_msg_array_size = *utils_msg_array_size + 10;
            *utils_msg_array = realloc(*utils_msg_array, *utils_msg_array_size * sizeof(char *));
        }

        va_copy(args_copy, args);
        msg_len = snprintf(msg_buf, MAX_MSG_SIZE, "%s:%d: %s: ", utils_file_name, line_no, type);
        msg_len += vsnprintf(msg_buf + msg_len, MAX_MSG_SIZE, format, args_copy);
        va_end(args_copy);

        (*utils_msg_array)[*utils_msg_count] = strdup(msg_buf);
        (*utils_msg_count)++;
    }
}

void print_error_msg(int line_no, const char * format, ...)
{
    va_list args;
    va_start(args, format);

    print_msg("error", line_no, format, args);

    va_end(args);
}

void print_warning_msg(int line_no, const char * format, ...)
{
    va_list args;
    va_start(args, format);

    print_msg("warning", line_no, format, args);

    va_end(args);
}

