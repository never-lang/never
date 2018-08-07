#include "dim.h"
#include "param.h"
#include <stdlib.h>

dim * dim_new(char * id)
{
    dim * value = malloc(sizeof(dim));

    value->id = param_new_int(id);
    value->line_no = 0;

    return value;
}

void dim_delete(dim * value)
{
    if (value->id != NULL)
    {
        param_delete(value->id);
    }
    free(value);
}

dim_list_node * dim_list_node_new(dim * value)
{
    dim_list_node * node = (dim_list_node *)malloc(sizeof(dim_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void dim_list_node_delete(dim_list_node * node)
{
    if (node->value != NULL)
    {
        dim_delete(node->value);
    }
    free(node);
}

dim_list * dim_list_new()
{
    dim_list * list = malloc(sizeof(dim_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void dim_list_delete(dim_list * list)
{
    dim_list_node * node = list->tail;

    while (node != NULL)
    {
        dim_list_node * tmp = node->next;
        dim_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void dim_list_add_beg(dim_list * list, dim * value)
{
    dim_list_node * node = dim_list_node_new(value);

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

void dim_list_add_end(dim_list * list, dim * value)
{
    dim_list_node * node = dim_list_node_new(value);

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
