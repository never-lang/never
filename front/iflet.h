#ifndef __IFLET_H__
#define __IFLET_H__

typedef struct symtab symtab;
typedef struct matchbind_list matchbind_list;
typedef struct enumtype enumtype;
typedef struct enumerator enumerator;
typedef struct expr expr;

typedef struct iflet
{
    char * enum_id;
    char * item_id;
    matchbind_list * matchbinds;
    expr * expr_value;
    expr * then_value;
    expr * else_value;
    enumtype * enumtype_value;
    enumerator * enumerator_value;
    unsigned int line_no;
} iflet;

iflet * iflet_new(char * enum_id, char * item_id, matchbind_list * matchbinds,
                  expr * expr_value, expr * then_value, expr * else_value);
void iflet_delete(iflet * value);

#endif /* __IFLET_H__ */

