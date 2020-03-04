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
#ifndef __DECL_H__
#define __DECL_H__

typedef enum decl_type
{
    DECL_TYPE_ENUMTYPE = 1,
    DECL_TYPE_RECORD = 2
} decl_type;

typedef struct enumtype enumtype;
typedef struct record record;

typedef struct decl
{
    decl_type type;
    union
    {
        enumtype * enumtype_value;
        record * record_value;
    };
} decl;

typedef struct decl_list_node
{
    decl * value;
    struct decl_list_node * prev;
    struct decl_list_node * next;
} decl_list_node;

typedef struct decl_list
{
    decl_list_node * head;
    decl_list_node * tail;
} decl_list;

decl * decl_new_enumtype(enumtype * value);
decl * decl_new_record(record * value);
void decl_delete(decl * value);

decl_list_node * decl_list_node_new(decl * value);
void decl_list_node_delete(decl_list_node * node);

decl_list * decl_list_new();
void decl_list_delete(decl_list * list);

void decl_list_add_beg(decl_list * list, decl * value);
void decl_list_add_end(decl_list * list, decl * value);

#endif /* __DECL_H__ */

