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
#ifndef __MATCHBIND_H__
#define __MATCHBIND_H__

typedef struct param param;
typedef struct enumerator enumerator;
typedef struct enumtype enumtype;

typedef struct matchbind
{
    char * id;
    param * param_value;
    enumerator * enumerator_value;
    enumtype * enumtype_value;
    int index;
    int stack_level;
    unsigned int line_no;
} matchbind;

typedef struct matchbind_list_node
{
    matchbind * value;
    struct matchbind_list_node * prev;
    struct matchbind_list_node * next;
} matchbind_list_node;

typedef struct matchbind_list
{
    unsigned int count;
    matchbind_list_node * head;
    matchbind_list_node * tail;
} matchbind_list;

matchbind * matchbind_new(char * value);
void matchbind_delete(matchbind * value);

void matchbind_print(matchbind * value);

matchbind_list_node * matchbind_list_node_new(matchbind * value);
void matchbind_list_node_delete(matchbind_list_node * node);

matchbind_list * matchbind_list_new();
void matchbind_list_delete(matchbind_list * list);

void matchbind_list_add_beg(matchbind_list * list, matchbind * value);
void matchbind_list_add_end(matchbind_list * list, matchbind * value);
void matchbind_list_set_stack_level(matchbind_list * list, int stack_level);

#endif /* __MATCHBIND_H__ */

