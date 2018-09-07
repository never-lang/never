#include "weakfunc.h"
#include <stdlib.h>

func_list_weak_node * func_list_weak_node_new(func * value)
{
    func_list_weak_node * node =
        (func_list_weak_node *)malloc(sizeof(func_list_weak_node));

    node->value = value;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void func_list_weak_node_delete(func_list_weak_node * node) { free(node); }

func_list_weak * func_list_weak_new()
{
    func_list_weak * list = (func_list_weak *)malloc(sizeof(func_list_weak));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void func_list_weak_delete(func_list_weak * list)
{
    func_list_weak_node * node = list->tail;
    while (node != NULL)
    {
        func_list_weak_node * tmp = node->next;
        func_list_weak_node_delete(node);
        node = tmp;
    }
    free(list);
}

void func_list_weak_add(func_list_weak * list, func * value)
{
    func_list_weak_node * node = func_list_weak_node_new(value);

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

func * func_list_weak_pop(func_list_weak * list)
{
    func_list_weak_node * head = NULL;

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

    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        func * value = head->value;
        func_list_weak_node_delete(head);
        return value;
    }
}

