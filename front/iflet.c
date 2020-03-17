#include "iflet.h"
#include "matchbind.h"
#include "expr.h"
#include <stdlib.h>

iflet * iflet_new(char * enum_id, char * item_id, matchbind_list * matchbinds,
                  expr * expr_value, expr * then_value, expr * else_value)
{
    iflet * ret = (iflet *)malloc(sizeof(iflet));
    
    ret->enum_id = enum_id;
    ret->item_id = item_id;
    ret->matchbinds = matchbinds;
    ret->expr_value = expr_value;
    ret->then_value = then_value;
    ret->else_value = else_value;
    ret->enumtype_value = NULL;
    ret->enumerator_value = NULL;
    ret->line_no = 0;
    
    return ret;
}

void iflet_delete(iflet * value)
{
    if (value->enum_id != NULL)
    {
        free(value->enum_id);
    }
    if (value->item_id != NULL)
    {
        free(value->item_id);
    }
    if (value->matchbinds != NULL)
    {
        matchbind_list_delete(value->matchbinds);
    }
    if (value->expr_value != NULL)
    {
        expr_delete(value->expr_value);
    }
    if (value->then_value != NULL)
    {
        expr_delete(value->then_value);
    }
    if (value->else_value != NULL)
    {
        expr_delete(value->else_value);
    }

    free(value);
}


