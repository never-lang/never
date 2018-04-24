#include <stdlib.h>
#include <stdio.h>
#include "expr.h"
#include "func.h"

expr * expr_new_int(int int_value)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = EXPR_INT;
    ret->int_value = int_value;
    ret->comb = COMB_TYPE_INT;
    
    return ret;
}

expr * expr_new_id(char * id)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = EXPR_ID;
    ret->id = id;
    ret->comb = COMB_TYPE_UNKNOWN;
    
    return ret;
}

expr * expr_new_one(int type, expr * left)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = type;
    ret->comb = COMB_TYPE_UNKNOWN;
    ret->left = left;
    ret->middle = NULL;
    ret->right = NULL;
    
    return ret;
}

expr * expr_new_two(int type, expr * left, expr * right)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = type;
    ret->comb = COMB_TYPE_UNKNOWN;
    ret->left = left;
    ret->middle = NULL;
    ret->right = right;
    
    return ret;
}

expr * expr_new_three(int type, expr * left, expr * middle, expr * right)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = type;
    ret->comb = COMB_TYPE_UNKNOWN;
    ret->left = left;
    ret->middle = middle;
    ret->right = right;
    
    return ret;
}

expr * expr_new_func(func * value)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = EXPR_FUNC;
    ret->comb = COMB_TYPE_UNKNOWN;
    ret->func_value = value;
    
    return ret;
}

expr * expr_new_call(expr * func_expr, expr_list * vars)
{
    expr * ret = (expr *) malloc(sizeof(expr));
    
    ret->type = EXPR_CALL;
    ret->comb = COMB_TYPE_UNKNOWN;
    ret->func_expr = func_expr;
    ret->vars = vars;
    
    return ret;
}

void expr_delete(expr * value)
{
    switch (value->type)
    {
        case EXPR_INT:
        break;
        case EXPR_ID:
            free(value->id);
        break;
        case EXPR_NEG:
            expr_delete(value->left);
        break;
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_LTE:
        case EXPR_GTE:
        case EXPR_EQ:
            expr_delete(value->left);
            expr_delete(value->right);
        break;
        case EXPR_SUP: /* ( expr ) */
            expr_delete(value->left);
        break;
        case EXPR_COND:
            expr_delete(value->left);
            expr_delete(value->middle);
            expr_delete(value->right);
        break;
        case EXPR_CALL:
            if (value->func_expr != NULL)
            {
                expr_delete(value->func_expr);
            }
            if (value->vars != NULL)
            {
                expr_list_delete(value->vars);
            }
        break;
        case EXPR_FUNC:
            if (value->func_value)
            {
                func_delete(value->func_value);
            }
        break;
    }

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
    if (node->value != NULL)
    {
        expr_delete(node->value);
    }
    free(node);
}

expr_list * expr_list_new()
{
    expr_list * list = (expr_list *)malloc(sizeof(expr_list));
    
    list->count = 0;
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
    free(list);    
}

void expr_list_add_beg(expr_list * list, expr * value)
{
    expr_list_node * node = expr_list_node_new(value);
    
    list->count++;
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

    list->count++;    
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

const char * comb_type_str(comb_type type)
{
    switch (type)
    {
        case COMB_TYPE_UNKNOWN: return "unknown";
        case COMB_TYPE_ERR: return "error";
        case COMB_TYPE_VOID: return "void";
        case COMB_TYPE_BOOL: return "bool";
        case COMB_TYPE_INT: return "int";
        case COMB_TYPE_FUNC: return "func";
    }
    return "unknown comb type!";
}



