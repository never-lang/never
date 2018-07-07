#ifndef __WEAKEXPR_H__
#define __WEAKEXPR_H__

#include "expr.h"

typedef struct expr_list_weak_node
{
    expr * value;
    int distance;
    struct expr_list_weak_node * next;
    struct expr_list_weak_node * prev;
} expr_list_weak_node;

typedef struct expr_list_weak
{
    int count;
    struct expr_list_weak_node * tail;
    struct expr_list_weak_node * head;
} expr_list_weak;

expr_list_weak_node * expr_list_weak_node_new(expr * value, int distance);
void expr_list_weak_node_delete(expr_list_weak_node * node);

expr_list_weak * expr_list_weak_new();
void expr_list_weak_delete(expr_list_weak * list);

void expr_list_weak_add(expr_list_weak * list, expr * value, int distance);
expr_list_weak_node * expr_list_weak_pop(expr_list_weak * list);


#endif /* __WEAKEXPR_H__ */

