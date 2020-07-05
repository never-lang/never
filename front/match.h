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
#ifndef MATCH_GUARD_H
#define MATCH_GUARD_H

typedef enum match_guard_type
{
    MATCH_GUARD_ITEM = 1,
    MATCH_GUARD_RECORD = 2,
    MATCH_GUARD_ELSE = 3
} match_guard_type;

typedef struct expr expr;
typedef struct enumtype enumtype;
typedef struct enumerator enumerator;
typedef struct matchbind_list matchbind_list;
typedef struct symtab symtab;

typedef struct match_guard_item
{
    char * enum_id;
    char * item_id;
    char * module_id;
    enumtype * enumtype_value;
    enumerator * enumerator_value;
    unsigned int line_no;
} match_guard_item;

typedef struct match_guard_item_expr
{
    match_guard_item * guard;
    expr * expr_value;
} match_guard_item_expr;

typedef struct match_guard_record
{
    symtab * stab;
    char * enum_id;
    char * item_id;
    char * module_id;
    matchbind_list * matchbinds;
    enumtype * enumtype_value;
    enumerator * enumerator_value;
    unsigned int line_no;
} match_guard_record;

typedef struct match_guard_record_expr
{
    match_guard_record * guard;
    expr * expr_value;
} match_guard_record_expr;

typedef struct match_guard_else_expr
{
    expr * expr_value;
    unsigned int line_no;
} match_guard_else_expr;

typedef struct match_guard
{
    match_guard_type type;
    union {
        match_guard_item_expr guard_item;
        match_guard_record_expr guard_record;
        match_guard_else_expr guard_else;
    };
    unsigned int line_no;
} match_guard;

typedef struct match_guard_list_node
{
    match_guard * value;
    struct match_guard_list_node * prev;
    struct match_guard_list_node * next;
} match_guard_list_node;

typedef struct match_guard_list
{
    int count;
    match_guard_list_node * head;
    match_guard_list_node * tail;
} match_guard_list;

match_guard_item * match_guard_item_new(char * enum_id, char * item_id);
match_guard_item * match_guard_item_new_module(char * enum_id, char * item_id, char * module_id);
void match_guard_item_delete(match_guard_item * value);

match_guard_record * match_guard_record_new(char * enum_id, char * item_id, matchbind_list * matchbinds);
match_guard_record * match_guard_record_new_module(char * enum_id, char * item_id, char * module_id, matchbind_list * matchbinds);
void match_guard_record_delete(match_guard_record * value);

match_guard * match_guard_new_item(match_guard_item * guard, expr * expr_value);
match_guard * match_guard_new_record(match_guard_record * guard, expr * expr_value);
match_guard * match_guard_new_else(expr * expr_value);
void match_guard_delete(match_guard * value);

expr * match_guard_get_expr(match_guard * value);

match_guard_list_node * match_guard_list_node_new(match_guard * value);
void match_guard_list_node_delete(match_guard_list_node * node);

match_guard_list * match_guard_list_new();
void match_guard_list_delete(match_guard_list * list);

void match_guard_list_add_beg(match_guard_list * list, match_guard * value);
void match_guard_list_add_end(match_guard_list * list, match_guard * value);
char match_guard_list_is_first(match_guard_list * list, match_guard_list_node * node);
char match_guard_list_is_last(match_guard_list * list, match_guard_list_node * node);

const char * match_guard_type_str(match_guard_type type);

#endif /* MATCH_GUARD_H */

