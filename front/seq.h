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
#ifndef __SEQ_H__
#define __SEQ_H__

typedef struct symtab symtab;
typedef struct bind bind;
typedef struct func func;
typedef struct expr expr;

typedef enum seq_type
{
    SEQ_TYPE_UNKNOWN = 0,
    SEQ_TYPE_BIND = 1,
    SEQ_TYPE_FUNC = 2,
    SEQ_TYPE_EXPR = 3
} seq_type;

typedef struct seq_item
{
    seq_type type;
    union
    {
        bind * bind_value;
        func * func_value;
        expr * expr_value;
    };
    unsigned int line_no;
} seq_item;

typedef struct seq_list_node
{
    seq_item * value;
    struct seq_list_node * prev;
    struct seq_list_node * next;
} seq_list_node;

typedef struct seq_list
{
    unsigned int count;
    seq_list_node * head;
    seq_list_node * tail;
} seq_list;

typedef struct seq
{
    symtab * stab;
    seq_list * list;
    unsigned int line_no;
} seq;

seq_item * seq_item_new_bind(bind * bind_value);
seq_item * seq_item_new_func(func * func_value);
seq_item * seq_item_new_expr(expr * expr_value);
void seq_item_delete(seq_item * value);

const char * seq_type_str(seq_type type);

seq_list_node * seq_list_node_new(seq_item * value);
void seq_list_node_delete(seq_list_node * node);

seq_list * seq_list_new();
void seq_list_delete(seq_list * list);

seq * seq_new(seq_list * list);
void seq_delete(seq * value);

void seq_list_add_beg(seq_list * list, seq_item * value);
void seq_list_add_end(seq_list * list, seq_item * value);

#endif /* __SEQ_H__*/
