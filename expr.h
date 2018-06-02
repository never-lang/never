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
#ifndef __EXPR_H__
#define __EXPR_H__

#include "freevar.h"

typedef float func_float_float(float x);

typedef enum expr_type
{
    EXPR_INT = 1,
    EXPR_FLOAT = 2,
    EXPR_ID  = 3,
    EXPR_NEG = 4,
    EXPR_ADD = 5,
    EXPR_SUB = 6,
    EXPR_MUL = 7,
    EXPR_DIV = 8,
    EXPR_LT  = 9,
    EXPR_GT  = 10,
    EXPR_LTE = 11,
    EXPR_GTE = 12,
    EXPR_EQ  = 13,
    EXPR_NEQ = 14,
    EXPR_SUP = 15, /* ( expr ) */
    EXPR_COND = 16, /* expr ? expr : expr */
    EXPR_CALL = 17, /* ID ( expr_list) */
    EXPR_FUNC = 18,  /* func ID ( ... ) */
    EXPR_BUILD_IN = 19,
    EXPR_INT_TO_FLOAT = 20,
    EXPR_FLOAT_TO_INT = 21  
} expr_type;

typedef enum comb_type
{
    COMB_TYPE_UNKNOWN = 0,
    COMB_TYPE_ERR  = 1,
    COMB_TYPE_VOID = 2,
    COMB_TYPE_BOOL = 3,
    COMB_TYPE_INT  = 4,
    COMB_TYPE_FLOAT  = 5,
    COMB_TYPE_FUNC = 6
} comb_type;

typedef enum id_type
{
    ID_TYPE_UNKNOWN = 0,
    ID_TYPE_LOCAL = 1,
    ID_TYPE_GLOBAL = 2,
    ID_TYPE_FUNC_TOP =  3,
    ID_TYPE_FUNC = 4,
    ID_TYPE_FUNC_NEST = 5
} id_type;

typedef struct func func;
typedef struct expr_list expr_list;

typedef struct expr
{
    expr_type type;
    comb_type comb;
    struct var_list * comb_vars; /* function arguments */
    struct var * comb_ret; /* function ret */
    unsigned int line_no;
    union
    {
        int int_value; /* EXPR_INT */
        float float_value; /* EXPR_FLOAT */
        struct /* EXPR_ID */
        {
            char * id;
            id_type id_type_value;
            union
            {
                var * id_var_value;
                freevar * id_freevar_value;
                func * id_func_value;
            };
        } id;
        struct
        {
            struct func * func_value; /* EXPR_FUNC */
        };
        struct
        {
            struct expr * left;
            struct expr * right;
            struct expr * middle; /* in ternary left_expr ? middle_expr : right_expr */
        };
        struct
        {
            struct expr * func_expr; /* EXPR_CALL */
            struct expr_list * vars;
        };
        struct
        {
            unsigned int id; /* EXPR_BUILD_IN */
            struct expr_list * param;
        } func_build_in;
    };
} expr;

typedef struct expr_list_node
{
    expr * value;
    struct expr_list_node * prev;
    struct expr_list_node * next;
} expr_list_node;

typedef struct expr_list
{
    int count;
    expr_list_node * head;
    expr_list_node * tail;
} expr_list;

expr * expr_new_int(int int_value);
expr * expr_new_float(float float_value);
expr * expr_new_id(char * id);
expr * expr_new_one(int type, expr * expr_left);
expr * expr_new_two(int type, expr * expr_left, expr * expr_right);
expr * expr_new_three(int type, expr * expr_left, expr * expr_middle, expr * expr_right);
expr * expr_new_func(func * value); 
expr * expr_new_call(expr * func_expr, expr_list * vars);
expr * expr_new_build_in(unsigned int id, expr_list * params);
expr * expr_new_conv(expr * expr_value, expr_type conv);

void expr_delete(expr * value);

expr_list_node * expr_list_node_new(expr * value);
void expr_list_node_delete(expr_list_node * node);

expr_list * expr_list_new();
void expr_list_delete(expr_list * list);

void expr_list_add_beg(expr_list * list, expr * value);
void expr_list_add_end(expr_list * list, expr * value);

const char * comb_type_str(comb_type type);

#endif /* __EXPR_H__ */


