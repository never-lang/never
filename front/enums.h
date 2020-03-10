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
#ifndef __ENUMS_H__
#define __ENUMS_H__

typedef struct param param;
typedef struct symtab symtab;
typedef struct enumerator enumerator;
typedef struct enumerator_list enumerator_list;

typedef enum enumtype_type
{
    ENUMTYPE_TYPE_PLAIN = 1,
    ENUMTYPE_TYPE_TAGGED = 2
} enumtype_type;

typedef struct enumtype
{
    enumtype_type type;
    char * id;
    enumerator_list * enums;
    symtab * stab;
    unsigned int line_no;
} enumtype;

enumtype * enumtype_new(char * id, enumerator_list * enums);
void enumtype_delete(enumtype * value);

enumerator * enumtype_find_enumerator(enumtype * enumtype_value, char * id);

#endif /* __ENUMS_H__ */


