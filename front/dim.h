#ifndef __DIM_H__
#define __DIM_H__

typedef struct dim
{
    struct param * id;
    unsigned int line_no;
} dim;

typedef struct dim_list_node
{
    dim * value;
    struct dim_list_node * prev;
    struct dim_list_node * next;
} dim_list_node;

typedef struct dim_list
{
    unsigned int count;
    dim_list_node * head;
    dim_list_node * tail;
} dim_list;

dim * dim_new(char * id);
void dim_delete(dim * value);

dim_list_node * dim_list_node_new(dim * value);
void dim_list_node_delete(dim_list_node * value);

dim_list * dim_list_new();
void dim_list_delete(dim_list * value);

void dim_list_add_beg(dim_list * list, dim * value);
void dim_list_add_end(dim_list * list, dim * value);

#endif /* __DIM_H__ */
