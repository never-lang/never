#ifndef __EXPR_H__
#define __EXPR_H__

#define EXPR_INT  1
#define EXPR_STR  2
#define EXPR_NEG  3
#define EXPR_ADD  4
#define EXPR_SUB  5
#define EXPR_MUL  6
#define EXPR_DIV  7
#define EXPR_LT   8
#define EXPR_GT   9
#define EXPR_LTE  10
#define EXPR_GTE  11
#define EXPR_EQ   12
#define EXPR_SUP  13 /* ( expr ) */
#define EXPR_COND 14

typedef struct expr
{
    int type;
    union
    {
        int int_value;
        char * str_value;
        struct
        {
            struct expr * left_value;
            struct expr * right_value;
            struct expr * middle_value; /* in ternary left_expr ? middle_expr : right_expr */
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
expr * expr_new_str(char * str_value);
expr * expr_new_one(int type, expr * expr_left);
expr * expr_new_two(int type, expr * expr_left, expr * expr_right);
expr * expr_new_three(int type, expr * expr_left, expr * expr_middle, expr * expr_right);

void expr_delete(expr * value);

expr_list_node * expr_list_node_new(expr * value);
void expr_list_node_delete(expr_list_node * node);

expr_list * expr_list_new();
void expr_list_delete(expr_list * list);

void expr_list_add_beg(expr_list * list, expr * value);
void expr_list_add_end(expr_list * list, expr * value);


#endif /* __EXPR_H__ */


