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
#ifndef __PARAM_H__
#define __PARAM_H__

#include <stdbool.h>

#define PARAM_CMP_SUCC 0
#define PARAM_CMP_FAIL 1

typedef struct range range;
typedef struct range_list range_list;

typedef enum param_type
{
    PARAM_BOOL = 1,
    PARAM_INT,
    PARAM_LONG,
    PARAM_FLOAT,
    PARAM_DOUBLE,
    PARAM_CHAR,
    PARAM_STRING,
    PARAM_VOID,
    PARAM_C_PTR,
    PARAM_DIM,
    PARAM_ARRAY,
    PARAM_RANGE,
    PARAM_RANGE_DIM,
    PARAM_SLICE,
    PARAM_SLICE_DIM,
    PARAM_ENUMTYPE,
    PARAM_RECORD,
    PARAM_FUNC
} param_type;

typedef enum param_const_type
{
    PARAM_CONST_TYPE_UNKNOWN = 0,
    PARAM_CONST_TYPE_CONST,
    PARAM_CONST_TYPE_VAR
} param_const_type;

typedef struct param
{
    param_type type;
    param_const_type const_type;
    int index;
    char * id;
    union {
        struct param * array;
        struct param * range;
        struct param * slice;
        struct param_list * params;
        struct param_list * dims;
        struct range_list * ranges;
        struct {
            char * record_id;
            char * module_id;
            union {
                struct record * record_value;
                struct enumtype * enumtype_value;
            };
        };
    };
    struct param * ret;
    unsigned int line_no;
} param;

typedef struct param_list_node
{
    param * value;
    struct param_list_node * prev;
    struct param_list_node * next;
} param_list_node;

typedef struct param_list
{
    unsigned int count;
    param_list_node * head;
    param_list_node * tail;
} param_list;

param * param_new_bool(char * id);
param * param_new_const_bool(char * id);
param * param_new_int(char * id);
param * param_new_const_int(char * id);
param * param_new_long(char * id);
param * param_new_const_long(char * id);
param * param_new_float(char * id);
param * param_new_const_float(char * id);
param * param_new_double(char * id);
param * param_new_const_double(char * id);
param * param_new_char(char * id);
param * param_new_const_char(char * id);
param * param_new_string(char * id);
param * param_new_const_string(char * id);
param * param_new_void(char * id);
param * param_new_const_void(char * id);
param * param_new_c_ptr(char * id);
param * param_new_const_c_ptr(char * id);
param * param_new_dim(char * id);
param * param_new_array(char * id, param_list * dims, param * ret);
param * param_new_range(char * id, range_list * ranges);
param * param_new_range_dim(char * id);
param * param_new_slice(char * id, range_list * ranges, param * ret);
param * param_new_record(char * id, char * record_id);
param * param_new_record_module(char * id, char * record_id, char * module_id);
param * param_new_func(char * id, param_list * params, param * ret);
void param_delete(param * value);

int param_cmp(param * param_one, param * param_two, bool const_cmp);
int param_list_cmp(param_list * param_one, param_list * param_two, bool const_cmp);
int func_cmp(param_list * param_list_one, param * ret_one,
             param_list * param_list_two, param * ret_two, bool const_cmp);
void param_init_const(param * value, param_const_type const_type);

param_list_node * param_list_node_new(param * value);
void param_list_node_delete(param_list_node * node);

param_list * param_list_new();
void param_list_delete(param_list * list);

void param_list_add_beg(param_list * list, param * value);
void param_list_add_end(param_list * list, param * value);

param * param_list_find(param_list * list, char * id);

void param_dim_set_array(param_list * dims, param * array);
void param_list_init_const(param_list * list, param_const_type const_type);

void param_print(param * value);
void param_list_print(param_list * list);
char * param_type_str(param_type type);

#endif /* __PARAM_H__ */
