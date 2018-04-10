#ifndef __EXPR_H__
#define __EXPR_H__

typedef struct expr
{
    int oper;
    struct expr * left;
    struct expr * right;
    struct expr * middle; /* in ternary left_expr ? middle_expr : right_expr */
} expr;

typedef struct expr_list
{
} expr_list;

#endif /* __EXPR_H__ */

