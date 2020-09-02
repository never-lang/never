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
#ifndef __ENUMERATOR_ID_H__
#define __ENUMERATOR_ID_H__

typedef enum enumerator_type
{
    ENUMERATOR_TYPE_ITEM = 1,
    ENUMERATOR_TYPE_VALUE = 2,
    ENUMERATOR_TYPE_RECORD = 3
} enumerator_type;

typedef struct record record;
typedef struct expr expr;

typedef struct enumerator
{
    enumerator_type type;
    char mark;
    char * id;
    record * record_value;
    expr * expr_value;
    unsigned int index;
    unsigned int line_no;
} enumerator;

typedef struct enumerator_list_node
{
    enumerator * value;
    struct enumerator_list_node * prev;
    struct enumerator_list_node * next;
} enumerator_list_node;

typedef struct enumerator_list
{
    enumerator_list_node * head;
    enumerator_list_node * tail;
} enumerator_list;

enumerator * enumerator_new(char * id);
enumerator * enumerator_new_expr(char * id, expr * expr_value);
enumerator * enumerator_new_record(char * id, record * record_value);
void enumerator_delete(enumerator * value);

enumerator_list_node * enumerator_list_node_new(enumerator * value);
void enumerator_list_node_delete(enumerator_list_node * node);

enumerator_list * enumerator_list_new();
void enumerator_list_delete(enumerator_list * list);

void enumerator_list_add_beg(enumerator_list * list, enumerator * value);
void enumerator_list_add_end(enumerator_list * list, enumerator * value);

void enumerator_list_unmark(enumerator_list * list);

#endif /* __ENUMERATOR_ID_H__ */


