#include "record.h"
#include "param.h"
#include <stdlib.h>

record * record_new(char * id, param_list * list)
{
    record * value = (record *)malloc(sizeof(record));
    
    value->id = id;
    value->list = list;
    
    return value;
}

void record_delete(record * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }

    if (value->list != NULL)
    {
        param_list_delete(value->list);
    }

    free(value);
}

record_list_node * record_list_node_new(record * value)
{
    record_list_node * node =
        (record_list_node *)malloc(sizeof(record_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void record_list_node_delete(record_list_node * node)
{
    if (node->value)
    {
        record_delete(node->value);
    }
    free(node);
}

record_list * record_list_new()
{
    record_list * list = (record_list *)malloc(sizeof(record_list));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void record_list_delete(record_list * list)
{
    record_list_node * node = list->tail;

    while (node != NULL)
    {
        record_list_node * tmp = node->next;
        record_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void record_list_add_beg(record_list * list, record * value)
{
    record_list_node * node = record_list_node_new(value);

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

void record_list_add_end(record_list * list, record * value)
{
    record_list_node * node = record_list_node_new(value);

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

