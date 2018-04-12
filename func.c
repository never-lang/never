#include <stdlib.h>
#include "func.h"

func * func_new(char * id, arg_list * args, arg * ret, func_body * body)
{
    func * value = (func *)malloc(sizeof(func));
    
    value->id = id;
    value->args = args;
    value->ret = ret;
    value->body = body;
    
    return value;
}

void func_delete(func * value)
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
    if (value->body)
    {
        func_body_delete(value->body);
    }
    
    free(value);
}

func_body * func_body_new(func_list * funcs, expr * ret)
{
    func_body * body = (func_body *)malloc(sizeof(func_body));
    
    body->funcs = funcs;
    body->ret = ret;
    
    return body;
}

void func_body_delete(func_body * body)
{
    if (body->funcs)
    {
        func_list_delete(body->funcs);
    }
    if (body->ret)
    {
        expr_delete(body->ret);
    }
    free(body);
}

func_list_node * func_list_node_new(func * value)
{
    func_list_node * node = (func_list_node *)malloc(sizeof(func_list_node));
    
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void func_list_node_delete(func_list_node * node)
{
    if (node->value)
    {
        func_delete(node->value);
    }
    free(node);
}

func_list * func_list_new()
{
    func_list * list = (func_list *)malloc(sizeof(func_list));
    
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void func_list_delete(func_list * list)
{
    func_list_node * node = list->tail;
    
    while (node != NULL)
    {
        func_list_node * tmp = node->next;
        func_list_node_delete(node);
        node = tmp;
    }
    free(list);    
}

void func_list_add_beg(func_list * list, func * value)
{
    func_list_node * node = func_list_node_new(value);
    
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

void func_list_add_end(func_list * list, func * value)
{
    func_list_node * node = func_list_node_new(value);
    
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


