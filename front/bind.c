#include "bind.h"
#include <stdlib.h>

bind * bind_new_let(char * id, expr * expr_value)
{
    bind * value = (bind *)malloc(sizeof(bind));

    value->type = BIND_LET;
    value->id = id;
    value->expr_value = expr_value;
    value->index = 0;
    value->line_no = 0;

    return value;
}

bind * bind_new_var(char * id, expr * expr_value)
{
    bind * value = (bind *)malloc(sizeof(bind));

    value->type = BIND_VAR;
    value->id = id;
    value->expr_value = expr_value;
    value->index = 0;
    value->line_no = 0;

    return value;
}

void bind_delete(bind * value)
{
    if (value->id)
    {
        free(value->id);
    }
    if (value->expr_value)
    {
        expr_delete(value->expr_value);
    }

    free(value);
}

bind_list_node * bind_list_node_new(bind * value)
{
    bind_list_node * node = (bind_list_node *)malloc(sizeof(bind_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void bind_list_node_delete(bind_list_node * node)
{
    if (node->value)
    {
        bind_delete(node->value);
    }
    free(node);
}

bind_list * bind_list_new()
{
    bind_list * list = (bind_list *)malloc(sizeof(bind_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void bind_list_delete(bind_list * list)
{
    bind_list_node * node = list->tail;

    while (node != NULL)
    {
        bind_list_node * tmp = node->next;
        bind_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void bind_list_add_beg(bind_list * list, bind * value)
{
    bind_list_node * node = bind_list_node_new(value);

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

void bind_list_add_end(bind_list * list, bind * value)
{
    bind_list_node * node = bind_list_node_new(value);

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




