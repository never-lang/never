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
#ifndef __RECORD_H__
#define __RECORD_H__

typedef struct param param;
typedef struct param_list param_list;

typedef struct record
{
    char * id;
    param_list * params;
    unsigned int line_no;
} record;

typedef struct record_list_node
{
    record * value;
    struct record_list_node * prev;
    struct record_list_node * next;
} record_list_node;

typedef struct record_list
{
    record_list_node * head;
    record_list_node * tail;
} record_list;

record * record_new(char * id, param_list * list);
void record_delete(record * value);

param * record_find_param(record * value, char * id);

record_list_node * record_list_node_new(record * value);
void record_list_node_delete(record_list_node * node);

record_list * record_list_new();
void record_list_delete(record_list * list);

void record_list_add_beg(record_list * list, record * value);
void record_list_add_end(record_list * list, record * value);

#endif /* __RECORD_H__ */

