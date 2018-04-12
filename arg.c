#include <stdlib.h>
#include "arg.h"

arg * arg_new_int(char * id)
{
    arg * value = (arg *)malloc(sizeof(arg));
    
    value->type = ARG_INT;
    value->id = id;
    value->args = NULL;
    value->ret = NULL;
    
    return value;
}

arg * arg_new_func(char * id, arg_list * args, arg * ret)
{
    arg * value = (arg *)malloc(sizeof(arg));
    
    value->type = ARG_INT;
    value->id = id;
    value->args = args;
    value->ret = ret;
    
    return value;
}

void arg_delete(arg * value)
{
    if (value->id)
    {
        free(value->id);
    }
    if (value->args)
    {
        arg_list_delete(value->args);
    }
    if (value->ret)
    {
        arg_delete(value->ret);
    }

    free(value);
}

arg_list_node * arg_list_node_new(arg * value)
{
    arg_list_node * node = (arg_list_node *)malloc(sizeof(arg_list_node));
    
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void arg_list_node_delete(arg_list_node * node)
{
    if (node->value)
    {
        arg_delete(node->value);
    }
    free(node);
}

arg_list * arg_list_new()
{
    arg_list * list = (arg_list *)malloc(sizeof(arg_list));
    
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void arg_list_delete(arg_list * list)
{
    arg_list_node * node = list->tail;
    
    while (node != NULL)
    {
        arg_list_node * tmp = node->next;
        arg_list_node_delete(node);
        node = tmp;
    }
    free(list);    
}

void arg_list_add_beg(arg_list * list, arg * value)
{
    arg_list_node * node = arg_list_node_new(value);
    
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

void arg_list_add_end(arg_list * list, arg * value)
{
    arg_list_node * node = arg_list_node_new(value);
    
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


