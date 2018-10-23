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
#ifndef __FREEVAR_H__
#define __FREEVAR_H__

#include "param.h"

typedef enum freevar_type
{
    FREEVAR_UNKNOWN = 0,
    FREEVAR_LOCAL = 1,
    FREEVAR_QUALIFIER = 2,
    FREEVAR_BIND = 3,
    FREEVAR_GLOBAL = 4,
    FREEVAR_FUNC = 5
} freevar_type;

typedef struct freevar
{
    freevar_type type;
    char * id;
    int index;
    union {
        struct param * local_value;
        struct bind * bind_value;
        struct qualifier * qualifier_value;
        struct freevar * global_value;
        struct func * func_value;
    };
} freevar;

typedef struct freevar_list_node
{
    freevar * value;
    struct freevar_list_node * prev;
    struct freevar_list_node * next;
} freevar_list_node;

typedef struct freevar_list
{
    int count;
    freevar_list_node * head;
    freevar_list_node * tail;
} freevar_list;

freevar * freevar_new(char * id, int index);
void freevar_delete(freevar * value);

freevar_list_node * freevar_list_node_new(freevar * value);
void freevar_list_node_delete(freevar_list_node * node);

freevar_list * freevar_list_new();
void freevar_list_delete(freevar_list * list);

freevar * freevar_list_add(freevar_list * list, char * id);

void freevar_list_add_beg(freevar_list * list, freevar * value);
void freevar_list_add_end(freevar_list * list, freevar * value);

void freevar_print(freevar * value);
void freevar_list_print(freevar_list * list);

char * freevar_type_str(freevar_type type);

#endif /* __FREEVAR_H__ */
