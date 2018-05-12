#ifndef __EXPR_H__
#define __EXPR_H__

#include "freevar.h"

typedef enum expr_type
{
    EXPR_INT = 1,
    EXPR_ID  = 2,
    EXPR_NEG = 3,
    EXPR_ADD = 4,
    EXPR_SUB = 5,
    EXPR_MUL = 6,
    EXPR_DIV = 7,
    EXPR_LT  = 8,
    EXPR_GT  = 9,
    EXPR_LTE = 10,
    EXPR_GTE = 11,
    EXPR_EQ  = 12,
    EXPR_SUP = 13, /* ( expr ) */
    EXPR_COND = 14, /* expr ? expr : expr */
    EXPR_CALL = 15, /* ID ( expr_list) */
    EXPR_FUNC = 16  /* func ID ( ... ) */
} expr_type;

typedef enum comb_type
{
    COMB_TYPE_UNKNOWN = 0,
    COMB_TYPE_ERR  = 1,
    COMB_TYPE_VOID = 2,
    COMB_TYPE_BOOL = 3,
    COMB_TYPE_INT  = 4,
    COMB_TYPE_FUNC = 5
} comb_type;

typedef enum id_type
{
    ID_TYPE_UNKNOWN = 0,
    ID_TYPE_LOCAL = 1,
    ID_TYPE_GLOBAL = 2,
    ID_TYPE_FUNC = 3,
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
        };
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
expr * expr_new_id(char * id);
expr * expr_new_one(int type, expr * expr_left);
expr * expr_new_two(int type, expr * expr_left, expr * expr_right);
expr * expr_new_three(int type, expr * expr_left, expr * expr_middle, expr * expr_right);
expr * expr_new_func(func * value); 
expr * expr_new_call(expr * func_expr, expr_list * vars);

void expr_delete(expr * value);

expr_list_node * expr_list_node_new(expr * value);
void expr_list_node_delete(expr_list_node * node);

expr_list * expr_list_new();
void expr_list_delete(expr_list * list);

void expr_list_add_beg(expr_list * list, expr * value);
void expr_list_add_end(expr_list * list, expr * value);

const char * comb_type_str(comb_type type);

#endif /* __EXPR_H__ */


