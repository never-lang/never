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
#ifndef __BIND_H__
#define __BIND_H__

#include "expr.h"

typedef enum bind_type
{
    BIND_UNKNOWN = 0,
    BIND_LET = 1,
    BIND_VAR = 2
} bind_type;

typedef struct bind
{
    bind_type type;
    char * id;
    expr * expr_value;
    int index;
    unsigned int line_no;
} bind;

typedef struct bind_list_node
{
    bind * value;
    struct bind_list_node * prev;
    struct bind_list_node * next;
} bind_list_node;

typedef struct bind_list
{
    unsigned int count;
    bind_list_node * head;
    bind_list_node * tail;
} bind_list;

bind * bind_new_let(char * id, expr * expr_value);
bind * bind_new_var(char * id, expr * expr_value);

void bind_delete(bind * value);

int bind_list_enum(bind_list * list, int start);

bind_list_node * bind_list_node_new(bind * value);
void bind_list_node_delete(bind_list_node * node);

bind_list * bind_list_new();
void bind_list_delete(bind_list * list);

void bind_list_add_beg(bind_list * list, bind * value);
void bind_list_add_end(bind_list * list, bind * value);

void bind_print(bind * value);

#endif /* __BIND_H__ */

