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

typedef struct enumtype
{
    char * id;
    enumerator_list * enums;
    symtab * stab;
    unsigned int line_no;
} enumtype;

typedef struct enumtype_list_node
{
    enumtype * value;
    struct enumtype_list_node * prev;
    struct enumtype_list_node * next;
} enumtype_list_node;

typedef struct enumtype_list
{
    enumtype_list_node * head;
    enumtype_list_node * tail;
} enumtype_list;

enumtype * enumtype_new(char * id, enumerator_list * enums);
void enumtype_delete(enumtype * value);

enumerator * enumtype_find_enumerator(enumtype * enumtype_value, char * id);

enumtype_list_node * enumtype_list_node_new(enumtype * value);
void enumtype_list_node_delete(enumtype_list_node * node);

enumtype_list * enumtype_list_new();
void enumtype_list_delete(enumtype_list * list);

void enumtype_list_add_beg(enumtype_list * list, enumtype * value);
void enumtype_list_add_end(enumtype_list * list, enumtype * value);

#endif /* __ENUMS_H__ */


