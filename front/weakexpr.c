#include <stdlib.h>
#include "weakexpr.h"

expr_list_weak_node * expr_list_weak_node_new(expr * value, int distance)
{
    expr_list_weak_node * node = (expr_list_weak_node *)malloc(sizeof(expr_list_weak_node));
    
    node->value = value;
    node->distance = distance;
    node->next = NULL;
    node->prev = NULL;
    
    return node;
}

void expr_list_weak_node_delete(expr_list_weak_node * node)
{
    free(node);
}

expr_list_weak * expr_list_weak_new()
{
    expr_list_weak * list = (expr_list_weak *)malloc(sizeof(expr_list_weak));
    
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void expr_list_weak_delete(expr_list_weak * list)
{
    expr_list_weak_node * node = list->tail;
    while (node != NULL)
    {
        expr_list_weak_node * tmp = node->next;
        expr_list_weak_node_delete(node);
        node = tmp;
    }
    free(list);
}

void expr_list_weak_add(expr_list_weak * list, expr * value, int distance)
{
    expr_list_weak_node * node = expr_list_weak_node_new(value, distance);
    
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

expr_list_weak_node * expr_list_weak_pop(expr_list_weak * list)
{
    expr_list_weak_node * head = NULL;
    
    head = list->head;
    if (head != NULL)
    {
        list->head = head->prev;
        if (list->head == NULL)
        {
            list->tail = NULL;
        }
        else
        {
            list->head->next = NULL;
        }
        list->count--;
    }
    
    return head;
}



