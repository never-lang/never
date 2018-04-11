#include <stdlib.h>
#include <stdio.h>
#include "expr.h"

expr * expr_new_int(int int_value)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = EXPR_INT;
    ret->int_value = int_value;
    
    return ret;
}

expr * expr_new_str(char * str_value)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = EXPR_STR;
    ret->str_value = str_value;
    
    return ret;
}

expr * expr_new_one(int type, expr * expr_left)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = type;
    ret->left_value = expr_left;
    ret->middle_value = NULL;
    ret->right_value = NULL;
    
    return ret;
}

expr * expr_new_two(int type, expr * expr_left, expr * expr_right)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = type;
    ret->left_value = expr_left;
    ret->middle_value = NULL;
    ret->right_value = expr_right;
    
    return ret;
}

expr * expr_new_three(int type, expr * expr_left, expr * expr_middle, expr * expr_right)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = type;
    ret->left_value = expr_left;
    ret->middle_value = expr_middle;
    ret->right_value = expr_right;
    
    return ret;
}

void expr_delete(expr * value)
{
    free(value);
}

expr_list_node * expr_list_node_new(expr * value)
{
    expr_list_node * node = (expr_list_node *)malloc(sizeof(expr_list_node));
    
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void expr_list_node_delete(expr_list_node * node)
{
    expr_delete(node->value);
    free(node);
}

expr_list * expr_list_new()
{
    expr_list * list = (expr_list *)malloc(sizeof(expr_list));
    
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void expr_list_delete(expr_list * list)
{
    expr_list_node * node = list->tail;
    
    while (node != NULL)
    {
        expr_list_node * tmp = node->next;
        expr_list_node_delete(node);
        node = tmp;
    }
    
    list->head = NULL;
    list->tail = NULL;
}

void expr_list_add_beg(expr_list * list, expr * value)
{
    expr_list_node * node = expr_list_node_new(value);
    
    if (list->head == NULL && list->tail == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->prev = node;
        node->next = list->tail;
        list->tail = node;
    }
}

void expr_list_add_end(expr_list * list, expr * value)
{
    expr_list_node * node = expr_list_node_new(value);
    
    if (list->head == NULL && list->tail == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->head->next = node;
        node->prev = list->head;
        list->head = node;
    }
}


