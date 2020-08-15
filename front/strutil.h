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
#ifndef __STRUTIL_H__
#define __STRUTIL_H__

typedef struct string {
    unsigned int pos;
    unsigned int size;
    char * buf;
} string;

string * string_new();
void string_delete(string * value);

char * string_take(string * value);

char * string_int(int val);
char * string_float(float val);
char * string_add(char * str_one, char * str_two);
char * string_add_int(char * str_one, int val, int order);
char * string_add_float(char * str_one, float val, int order);
char * string_add_long(char * str_one, long long val, int order);
char * string_add_double(char * str_one, double val, int order);
char * string_add_char(char * str_one, char val, int order);

void string_add_char_end(string * value, char c);

void string_print(char * val);
void string_print_int(int val);
void string_print_float(float val);
void string_print_char(char val);

#endif /* __STRUTIL_H__ */

