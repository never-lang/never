/** 
 * Copyright 2020 Slawomir Maludzinski
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
#ifndef __FORIN_H__
#define __FORIN_H__

typedef struct expr expr;
typedef struct symtab symtab;
typedef struct param param;

typedef enum forin_type
{
    FORIN_TYPE_ARRAY,
    FORIN_TYPE_SLICE,
    FORIN_TYPE_RANGE,
    FORIN_TYPE_STRING
} forin_type;

typedef struct forin
{
    forin_type type;
    expr * iter;
    struct expr * in_value;
    struct expr * do_value;
    int stack_level;
    unsigned int line_no;
    symtab * stab;
} forin;

forin * forin_new(char * id, expr * in_value, expr * do_value);
void forin_delete(forin * value);

void forin_print(forin * value);
char * forin_type_str(forin_type type);

#endif /* __FORIN_H__ */
