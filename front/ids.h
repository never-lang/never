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
#ifndef __IDS_H__
#define __IDS_H__

typedef struct id
{
    char * id;
    unsigned int line_no;
} id;

typedef struct id_list_node
{
    id * value;
    struct id_list_node * prev;
    struct id_list_node * next;
} id_list_node;

typedef struct id_list
{
    unsigned int count;
    id_list_node * head;
    id_list_node * tail;
} id_list;

id * id_new(char * value);
void id_delete(id * value);

id_list_node * id_list_node_new(id * value);
void id_list_node_delete(id_list_node * node);

id_list * id_list_new();
void id_list_delete(id_list * list);

void id_list_add_beg(id_list * list, id * value);
void id_list_add_end(id_list * list, id * value);

#endif /* __IDS_H__ */

