#include "listcomp.h"
#include "array.h"
#include "expr.h"
#include "symtab.h"
#include <stdlib.h>
#include <stdio.h>

qualifier * qualifier_new_generator(char * id, expr * expr_value)
{
    qualifier * value = (qualifier *)malloc(sizeof(qualifier));

    value->type = QUALIFIER_GENERATOR;
    value->id = id;
    value->expr_value = expr_value;
    value->stack_level = 0;
    value->line_no = 0;

    return value;
}

qualifier * qualifier_new_filter(expr * expr_value)
{
    qualifier * value = (qualifier *)malloc(sizeof(qualifier));

    value->type = QUALIFIER_FILTER;
    value->id = NULL;
    value->expr_value = expr_value;
    value->stack_level = 0;
    value->line_no = 0;

    return value;
}

void qualifier_delete(qualifier * value)
{
    switch (value->type)
    {
    case QUALIFIER_UNKNOWN:
        break;
    case QUALIFIER_GENERATOR:
        if (value->id != NULL)
        {
            free(value->id);
        }
        if (value->expr_value != NULL)
        {
            expr_delete(value->expr_value);
        }
        break;
    case QUALIFIER_FILTER:
        if (value->id != NULL)
        {
            free(value->id);
        }
        if (value->expr_value != NULL)
        {
            expr_delete(value->expr_value);
        }
        break;
    }
    free(value);
}

void qualifier_print(qualifier * value)
{
    printf("qualifier %s %s stack %d at %u\n", value->id,
                                      qualifier_type_str(value->type), 
                                      value->stack_level,
                                      value->line_no);
}

const char * qualifier_type_str(qualifier_type type)
{
    switch (type)
    {
        case QUALIFIER_UNKNOWN: return "QUALIFIER_UNKNOWN";
        case QUALIFIER_GENERATOR: return "QUALIFIER_GENERATOR";
        case QUALIFIER_FILTER: return "QUALIFIER_FILTER";
    }
    return "QUALIFIER_UNKNOWN";
}

qualifier_list_node * qualifier_list_node_new(qualifier * value)
{
    qualifier_list_node * node =
        (qualifier_list_node *)malloc(sizeof(qualifier_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void qualifier_list_node_delete(qualifier_list_node * node)
{
    if (node->value)
    {
        qualifier_delete(node->value);
    }
    free(node);
}

qualifier_list * qualifier_list_new()
{
    qualifier_list * list = (qualifier_list *)malloc(sizeof(qualifier_list));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void qualifier_list_delete(qualifier_list * list)
{
    qualifier_list_node * node = list->tail;

    while (node != NULL)
    {
        qualifier_list_node * tmp = node->next;
        qualifier_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void qualifier_list_add_beg(qualifier_list * list, qualifier * value)
{
    qualifier_list_node * node = qualifier_list_node_new(value);

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

void qualifier_list_add_end(qualifier_list * list, qualifier * value)
{
    qualifier_list_node * node = qualifier_list_node_new(value);

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

listcomp * listcomp_new(expr * expr_value, qualifier_list * list, param * ret)
{
    listcomp * value = (listcomp *)malloc(sizeof(listcomp));

    value->expr_value = expr_value;
    value->list = list;
    value->ret = ret;
    value->stab = NULL;

#if 0
    if (value->ret != NULL)
    {
        param_init_const(value->ret, PARAM_CONST_TYPE_VAR);
    }
#endif

    return value;
}

void listcomp_delete(listcomp * value)
{
    if (value->expr_value != NULL)
    {
        expr_delete(value->expr_value);
    }
    if (value->list != NULL)
    {
        qualifier_list_delete(value->list);
    }
    if (value->ret != NULL)
    {
        param_delete(value->ret);
    }
    if (value->stab != NULL)
    {
        symtab_delete(value->stab);
    }
    free(value);
}
