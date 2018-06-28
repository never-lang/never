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
#ifndef __VAR_H__
#define __VAR_H__

enum
{
    VAR_INT = 1,
    VAR_FLOAT = 2,
    VAR_FUNC = 3
};

typedef struct var
{
    int type;
    int index;
    char * id;
    struct var_list * vars;
    struct var * ret;
    unsigned int line_no;
} var;

typedef struct var_list_node
{
    var * value;
    struct var_list_node * prev;
    struct var_list_node * next;
} var_list_node;

typedef struct var_list
{
    int count;
    var_list_node * head;
    var_list_node * tail;
} var_list;

var * var_new_int(char * id);
var * var_new_float(char * id);
var * var_new_func(char * id, var_list * vars, var * ret);
void var_delete(var * value);

var_list_node * var_list_node_new(var * value);
void var_list_node_delete(var_list_node * node);

var_list * var_list_new();
void var_list_delete(var_list * list);

void var_list_add_beg(var_list * list, var * value);
void var_list_add_end(var_list * list, var * value);

void var_print(var * value);
void var_list_print(var_list * list);
char * var_type_str(int type);

#endif /* __VAR_H__ */
