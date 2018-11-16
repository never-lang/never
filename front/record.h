#ifndef __RECORD_H__
#define __RECORD_H__

typedef struct param param;
typedef struct param_list param_list;

typedef struct record
{
    char * id;
    param_list * params;
    unsigned int line_no;
} record;

typedef struct record_list_node
{
    record * value;
    struct record_list_node * prev;
    struct record_list_node * next;
} record_list_node;

typedef struct record_list
{
    record_list_node * head;
    record_list_node * tail;
} record_list;

record * record_new(char * id, param_list * list);
void record_delete(record * value);

param * record_find_param(record * value, char * id);

record_list_node * record_list_node_new(record * value);
void record_list_node_delete(record_list_node * node);

record_list * record_list_new();
void record_list_delete(record_list * list);

void record_list_add_beg(record_list * list, record * value);
void record_list_add_end(record_list * list, record * value);

#endif /* __RECORD_H__ */

