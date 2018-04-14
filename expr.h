#ifndef __EXPR_H__
#define __EXPR_H__

enum
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
    EXPR_COND = 14,
    EXPR_CALL = 15,
    EXPR_FUNC = 16
};

typedef struct func func;
typedef struct expr_list expr_list;

typedef struct expr
{
    int type;
    union
    {
        int int_value;
        char * id;
        struct func * func_value;
        struct
        {
            struct expr * left_value;
            struct expr * right_value;
            struct expr * middle_value; /* in ternary left_expr ? middle_expr : right_expr */
        };
        struct
        {
            char * func_id;
            struct expr_list * args;
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
    expr_list_node * head;
    expr_list_node * tail;
} expr_list;

expr * expr_new_int(int int_value);
expr * expr_new_id(char * id);
expr * expr_new_one(int type, expr * expr_left);
expr * expr_new_two(int type, expr * expr_left, expr * expr_right);
expr * expr_new_three(int type, expr * expr_left, expr * expr_middle, expr * expr_right);
expr * expr_new_func(func * value); 
expr * expr_new_call(char * func_id, expr_list * args);

void expr_delete(expr * value);

expr_list_node * expr_list_node_new(expr * value);
void expr_list_node_delete(expr_list_node * node);

expr_list * expr_list_new();
void expr_list_delete(expr_list * list);

void expr_list_add_beg(expr_list * list, expr * value);
void expr_list_add_end(expr_list * list, expr * value);


#endif /* __EXPR_H__ */


