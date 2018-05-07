#include <stdlib.h>
#include <stdio.h>
#include "var.h"

var * var_new_int(char * id)
{
    var * value = (var *)malloc(sizeof(var));
    
    value->type = VAR_INT;
    value->index = -1;
    value->id = id;
    value->vars = NULL;
    value->ret = NULL;
    
    return value;
}

var * var_new_func(char * id, var_list * vars, var * ret)
{
    var * value = (var *)malloc(sizeof(var));
    
    value->type = VAR_FUNC;
    value->id = id;
    value->vars = vars;
    value->ret = ret;
    
    return value;
}

void var_delete(var * value)
{
    if (value->id)
    {
        free(value->id);
    }
    if (value->vars)
    {
        var_list_delete(value->vars);
    }
    if (value->ret)
    {
        var_delete(value->ret);
    }

    free(value);
}

var_list_node * var_list_node_new(var * value)
{
    var_list_node * node = (var_list_node *)malloc(sizeof(var_list_node));
    
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void var_list_node_delete(var_list_node * node)
{
    if (node->value)
    {
        var_delete(node->value);
    }
    free(node);
}

var_list * var_list_new()
{
    var_list * list = (var_list *)malloc(sizeof(var_list));
    
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void var_list_delete(var_list * list)
{
    var_list_node * node = list->tail;
    
    while (node != NULL)
    {
        var_list_node * tmp = node->next;
        var_list_node_delete(node);
        node = tmp;
    }
    free(list);    
}

void var_list_add_beg(var_list * list, var * value)
{
    var_list_node * node = var_list_node_new(value);
    
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

void var_list_add_end(var_list * list, var * value)
{
    var_list_node * node = var_list_node_new(value);
    
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

void var_print(var * value)
{
    printf("var %s %d %s\n", var_type_str(value->type), value->index, value->id);
}

void var_list_print(var_list * list)
{
    var_list_node * node = list->tail;
    while (node != NULL)
    {
        var * value = node->value;
        if (value != NULL)
        {
            var_print(value);
        }
    
        node = node->next;
    }
}

char * var_type_str(int type)
{
    switch (type)
    {
        case VAR_INT: return "VAR_INT";
        case VAR_FUNC: return "VAR_FUNC";
    }
    return "VAR_???";
}



