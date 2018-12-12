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
#ifndef __TOK_ID_H__
#define __TOK_ID_H__

typedef struct tokid
{
    char * id;
    int index;
    unsigned int line_no;
} tokid;

typedef struct tokid_list_node
{
    tokid * value;
    struct tokid_list_node * prev;
    struct tokid_list_node * next;
} tokid_list_node;

typedef struct tokid_list
{
    tokid_list_node * head;
    tokid_list_node * tail;
} tokid_list;

tokid * tokid_new(char * id);
void tokid_delete(tokid * value);

tokid_list_node * tokid_list_node_new(tokid * value);
void tokid_list_node_delete(tokid_list_node * node);

tokid_list * tokid_list_new();
void tokid_list_delete(tokid_list * list);

void tokid_list_add_beg(tokid_list * list, tokid * value);
void tokid_list_add_end(tokid_list * list, tokid * value);

#endif /* __TOK_ID_H__ */


