#ifndef __VAR_H__
#define __VAR_H__

enum
{
    VAR_INT = 1,
    VAR_FUNC = 2
};

typedef struct var
{
    int type;
    int index;
    char * id;
    struct var_list * vars;
    struct var * ret;
} var;

typedef struct var_list_node
{
    var * value;
    struct var_list_node * prev;
    struct var_list_node * next;
} var_list_node;

typedef struct var_list
{
    int count;
    var_list_node * head;
    var_list_node * tail;
} var_list;

var * var_new_int(char * id);
var * var_new_func(char * id, var_list * vars, var * ret);
void var_delete(var * value);

var_list_node * var_list_node_new(var * value);
void var_list_node_delete(var_list_node * node);

var_list * var_list_new();
void var_list_delete(var_list * list);

void var_list_add_beg(var_list * list, var * value);
void var_list_add_end(var_list * list, var * value);

void var_print(var * value);
void var_list_print(var_list * list);
char * var_type_str(int type);

#endif /* __VAR_H__ */

