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
#include "strutil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRING_BUF_SIZE 32
#define STRING_CONV_BUF_SIZE 32

string * string_new()
{
    string * value = (string *)malloc(sizeof(string));
    
    value->pos = 0;
    value->size = STRING_BUF_SIZE;
    value->buf = (char *)malloc(value->size * sizeof(char));
    
    return value;
}

void string_delete(string * value)
{
    if (value->buf)
    {
        free(value->buf);
    }
    free(value);
}

void string_add_char(string * value, char c)
{
    value->buf[value->pos++] = c;

    if (value->pos >= value->size)
    {
        value->size += STRING_BUF_SIZE;
        value->buf = (char *)realloc(value->buf, value->size * sizeof(char));
    }
}

char * string_take(string * value)
{
    char * buf = value->buf;

    value->buf = NULL;
    return buf;
}

char * string_int(int val)
{
    char tmpstr[STRING_CONV_BUF_SIZE] = { 0 };

    sprintf(tmpstr, "%d", val);
    return strdup(tmpstr);
}

char * string_float(float val)
{
    char tmpstr[STRING_CONV_BUF_SIZE] = { 0 };

    sprintf(tmpstr, "%.2f", val);
    return strdup(tmpstr);
}

char * string_add(char * str_one, char * str_two)
{
    size_t left_len = 0;
    size_t right_len = 0;
    char * tmpstr = NULL; 

    left_len = strlen(str_one);
    right_len =  strlen(str_two);
    tmpstr = (char *) malloc((left_len + right_len + 1) * sizeof(char));

    strcpy(tmpstr, str_one);
    strcpy(tmpstr + left_len, str_two);
    
    return tmpstr;            
}

char * string_add_int(char * str_one, int val, int order)
{
    size_t str_len = 0;
    size_t int_len = 0;
    char * tmpstr = NULL;
    char str_int[STRING_CONV_BUF_SIZE] = { 0 };
    
    sprintf(str_int, "%d", val);
    str_len = strlen(str_one);
    int_len = strlen(str_int);
    tmpstr = (char *) malloc((str_len + int_len + 1) * sizeof(char));
    
    if (order)
    {
        strcpy(tmpstr, str_one);
        strcpy(tmpstr + str_len, str_int);
    }
    else
    {
        strcpy(tmpstr, str_int);
        strcpy(tmpstr + int_len, str_one);
    }
    
    return tmpstr;
}

char * string_add_float(char * str_one, float val, int order)
{
    size_t str_len = 0;
    size_t float_len = 0;
    char * tmpstr = NULL;
    char str_float[STRING_CONV_BUF_SIZE] = { 0 };
    
    sprintf(str_float, "%.2f", val);
    str_len = strlen(str_one);
    float_len = strlen(str_float);
    tmpstr = (char *) malloc((str_len + float_len + 1) * sizeof(char));
    
    if (order)
    {
        strcpy(tmpstr, str_one);
        strcpy(tmpstr + str_len, str_float);
    }
    else
    {
        strcpy(tmpstr, str_float);
        strcpy(tmpstr + float_len, str_one);
    }
    
    return tmpstr;
}

void string_print(char * val)
{
    printf("%s", val);
}

void string_print_int(int val)
{
    printf("%d\r\n", val);
}

void string_print_float(float val)
{
    printf("%.2f\r\n", val);
}

void string_print_char(char val)
{
    printf("%c", val);
}

