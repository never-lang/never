#ifndef __LISTCOMP_H__
#define __LISTCOMP_H__

typedef enum qualifier_type {
    QUALIFIER_UNKNOWN = 0,
    QUALIFIER_GENERATOR = 1,
    QUALIFIER_FILTER = 2,
} qualifier_type;

typedef struct array array;
typedef struct expr expr;
typedef struct param param;
typedef struct symtab symtab;

typedef struct qualifier
{
    qualifier_type type;
    char * id;
    expr * expr_value;
    int stack_level;
    unsigned int line_no;
} qualifier;

typedef struct qualifier_list_node
{
    qualifier * value;
    struct qualifier_list_node * prev;
    struct qualifier_list_node * next;
} qualifier_list_node;

typedef struct qualifier_list
{
    qualifier_list_node * head;
    qualifier_list_node * tail;
} qualifier_list;

typedef struct listcomp
{
    symtab * stab;
    expr * expr_value;
    qualifier_list * list;
    param * ret;
    int stack_level;
    unsigned int line_no;
} listcomp;

qualifier * qualifier_new_generator(char * id, expr * expr_value);
qualifier * qualifier_new_filter(expr * expr_value);
void qualifier_delete(qualifier * value);

void qualifier_print(qualifier * value);
const char * qualifier_type_str(qualifier_type type);

qualifier_list_node * qualifier_list_node_new(qualifier * value);
void qualifier_list_node_delete(qualifier_list_node * node);

qualifier_list * qualifier_list_new();
void qualifier_list_delete(qualifier_list * list);

void qualifier_list_add_beg(qualifier_list * list, qualifier * value);
void qualifier_list_add_end(qualifier_list * list, qualifier * value);

listcomp * listcomp_new(expr * expr_value, qualifier_list * list, param * ret);
void listcomp_delete(listcomp * value);

#endif /* __LISTCOMP_H__ */
