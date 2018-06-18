#ifndef __TCHECKARR_H__
#define __TCHECKARR_H__

#include "expr.h"
#include "symtab.h"

typedef struct expr_list_simple_node
{
    expr * value;
    int distance;
    struct expr_list_simple_node * next;
    struct expr_list_simple_node * prev;
} expr_list_simple_node;

typedef struct expr_list_simple
{
    int count;
    struct expr_list_simple_node * tail;
    struct expr_list_simple_node * head;
} expr_list_simple;

expr_list_simple_node * expr_list_simple_node_new(expr * value, int distance);
void expr_list_simple_node_delete(expr_list_simple_node * node);

expr_list_simple * expr_list_simple_new();
void expr_list_simple_delete(expr_list_simple * list);

void expr_list_simple_add(expr_list_simple * list, expr * value, int distance);
expr_list_simple_node * expr_list_simple_pop(expr_list_simple * list);

int array_to_depth_list(expr * value, expr_list_simple * depth_list);
int array_depth_list_well_formed(expr * expr_value, expr_list_simple * depth_list, int * result);
int array_set_dims(expr_list_simple * depth_list);
int array_well_formed(expr * value, int * result);

int array_check_type(symtab * tab, expr * value, int * result);
int expr_array_check_type(symtab * tab, expr * value, int * result);

#endif /* __TCHECKARR_H__ */

