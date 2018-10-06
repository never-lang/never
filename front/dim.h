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
#ifndef __DIM_H__
#define __DIM_H__

typedef struct dim
{
    struct param * id;
    unsigned int line_no;
} dim;

typedef struct dim_list_node
{
    dim * value;
    struct dim_list_node * prev;
    struct dim_list_node * next;
} dim_list_node;

typedef struct dim_list
{
    unsigned int count;
    dim_list_node * head;
    dim_list_node * tail;
} dim_list;

dim * dim_new(char * id);
void dim_delete(dim * value);

dim_list_node * dim_list_node_new(dim * value);
void dim_list_node_delete(dim_list_node * value);

dim_list * dim_list_new();
void dim_list_delete(dim_list * value);

void dim_list_add_beg(dim_list * list, dim * value);
void dim_list_add_end(dim_list * list, dim * value);

#endif /* __DIM_H__ */
