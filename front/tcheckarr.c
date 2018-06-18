#include <stdlib.h>
#include <stdio.h>
#include "tcheckarr.h"
#include "typecheck.h"
#include "utils.h"

expr_list_simple_node * expr_list_simple_node_new(expr * value, int distance)
{
    expr_list_simple_node * node = (expr_list_simple_node *)malloc(sizeof(expr_list_simple_node));
    
    node->value = value;
    node->distance = distance;
    node->next = NULL;
    node->prev = NULL;
    
    return node;
}

void expr_list_simple_node_delete(expr_list_simple_node * node)
{
    free(node);
}

expr_list_simple * expr_list_simple_new()
{
    expr_list_simple * list = (expr_list_simple *)malloc(sizeof(expr_list_simple));
    
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void expr_list_simple_delete(expr_list_simple * list)
{
    expr_list_simple_node * node = list->tail;
    while (node != NULL)
    {
        expr_list_simple_node * tmp = node->next;
        expr_list_simple_node_delete(node);
        node = tmp;
    }
    free(list);
}

void expr_list_simple_add(expr_list_simple * list, expr * value, int distance)
{
    expr_list_simple_node * node = expr_list_simple_node_new(value, distance);
    
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

expr_list_simple_node * expr_list_simple_pop(expr_list_simple * list)
{
    expr_list_simple_node * head = NULL;
    
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

int elements_to_depth_list(expr_list_simple * bfs_list, expr * value, int distance)
{
    expr_list_node * node = value->array.array_value->elements->tail;
    while (node != NULL)
    {
        expr * elem = node->value;
        if (elem != NULL)
        {
            expr_list_simple_add(bfs_list, elem, distance);
        }
        node = node->next;
    }

    return 0;
}

int array_to_depth_list(expr * value, expr_list_simple * depth_list)
{
    expr_list_simple * bfs_list = expr_list_simple_new();

    expr_list_simple_add(bfs_list, value, 0);
    elements_to_depth_list(bfs_list, value, 1);

    while (bfs_list->count > 0)
    {
        expr_list_simple_node * head = expr_list_simple_pop(bfs_list);
        expr * value = head->value;
        
        if (value->type == EXPR_ARRAY && 
            value->array.array_value->type == ARRAY_SUB)
        {
            elements_to_depth_list(bfs_list, value, head->distance + 1);
        }

        expr_list_simple_add(depth_list, value, head->distance + 1);
        expr_list_simple_node_delete(head);
    }

    expr_list_simple_delete(bfs_list);

    return 0;
}

int array_depth_list_well_formed(expr * expr_value, expr_list_simple * depth_list, int * result)
{
    int first_distance = - 1;
    int curr_distance = -1;
    int first_comb_dims = 0;
    int first_comb_elems = 0;
    var * ret = expr_value->array.array_value->ret;

    expr_list_simple_node * node = depth_list->tail;
    curr_distance = first_distance = node->distance;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            #if 0
            printf("depth expr type %s comb %s %d", expr_type_str(value->type),
                                                    comb_type_str(value->comb),
                                                    node->distance);
            if (value->type == EXPR_ARRAY)
            {
                printf(" count %d", value->array.array_value->elements->count);
            }
            printf("\n");
            #endif
            
            if (node->distance == first_distance)
            {
                if (var_expr_cmp(ret, value) == TYPECHECK_FAIL)
                {
                    *result = TYPECHECK_FAIL;
                    print_error_msg(value->line_no,
                           "incorrect types in array %s %s\n",
                           var_type_str(ret->type),
                           expr_type_str(value->type));
                }
            }
            else
            {
                if (node->distance != curr_distance)
                {
                    curr_distance = node->distance;

                    if (value->type == EXPR_ARRAY &&
                       (value->array.array_value->type == ARRAY_SUB ||
                          value->array.array_value->type == ARRAY_INIT ))
                    {
                        first_comb_dims = value->array.array_value->dims_count;
                        first_comb_elems = value->array.array_value->elements->count;
                    }
                    else
                    {
                        *result = TYPECHECK_FAIL;
                        print_error_msg(value->line_no,
                                       "incorrect types in array %s %s\n",
                                        var_type_str(ret->type),
                                        expr_type_str(value->type));
                    }
                }
                else
                {
                    if (value->type == EXPR_ARRAY &&
                       (value->array.array_value->type == ARRAY_SUB ||
                          value->array.array_value->type == ARRAY_INIT ))
                    {
                        if (value->array.array_value->dims_count != first_comb_dims ||
                            value->array.array_value->elements->count != first_comb_elems)
                        {
                            *result = TYPECHECK_FAIL;
                            print_error_msg(value->line_no,
                                           "incorrect dimesions in array %s %s\n",
                                            var_type_str(ret->type),
                                            expr_type_str(value->type));
                        }
                    }
                    else
                    {
                        *result = TYPECHECK_FAIL;
                        print_error_msg(value->line_no,
                                       "incorrect types in array %s %s\n",
                                        var_type_str(ret->type),
                                        expr_type_str(value->type));
                    }
                }
            }
        }
        node = node->next;
    }

    return 0;
}

int array_set_dims(expr_list_simple * depth_list)
{
    int dim = 0;
    int distance = 0;
    
    expr_list_simple_node * node = depth_list->tail;
    if (node != NULL)
    {
        dim = node->distance;
    }

    node = depth_list->head;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            if (node->distance != distance)
            {
                dim--;
                distance = node->distance;
            }
            if (value->type == EXPR_ARRAY)
            {
                value->array.array_value->dims_count = dim;
            }
            else
            {
                break;
            }
        }
        node = node->prev;
    }
    
    return 0;
}

int array_well_formed(expr * value, int * result)
{
    expr_list_simple * depth_list = expr_list_simple_new();
    array_to_depth_list(value, depth_list);

    array_depth_list_well_formed(value, depth_list, result);
    array_set_dims(depth_list);

    expr_list_simple_delete(depth_list);

    return 0;
}

int array_check_type(symtab * tab, expr * value, int * result)
{
    if (value->array.array_value->type == ARRAY_INIT)
    {
        if (value->array.array_value->elements != NULL)
        {
            expr_list_check_type(tab, value->array.array_value->elements, result);
        }

        array_well_formed(value, result);
        if (*result == TYPECHECK_FAIL)
        {
            print_error_msg(value->line_no, "array is not well formed\n");
        }
    }
    else if (value->array.array_value->type == ARRAY_SUB)
    {
        if (value->array.array_value->elements != NULL)
        {
            expr_list_check_type(tab, value->array.array_value->elements, result);
        }
    }
    else if (value->array.array_value->type == ARRAY_DIMS)
    {
        if (value->array.array_value->elements != NULL)
        {
            array_dims_check_type_expr_list(tab, value->array.array_value->elements, result);
            if (*result == TYPECHECK_SUCC)
            {
                value->array.array_value->dims_count = value->array.array_value->elements->count;
            }
        }
    }

    return 0;
}

int expr_array_check_type(symtab * tab, expr * value, int * result)
{
    int array_result = TYPECHECK_SUCC;

    array_check_type(tab, value, &array_result);

    if (array_result == TYPECHECK_SUCC)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_ret = value->array.array_value->ret;
        value->comb_dims = value->array.array_value->dims_count;
    }
    else
    {
        value->comb = COMB_TYPE_ERR;
    }

    return 0;
}

