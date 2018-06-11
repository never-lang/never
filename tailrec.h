#ifndef __TAILREC_H__
#define __TAILREC_H__

#include "never.h"

typedef enum
{
    TAILREC_NOT_FOUND = 0,
    TAILREC_FOUND = 1,
    TAILREC_NOT_POSSIBLE = 2
} tailrec_type;

typedef enum
{
    TAILREC_CHECK = 0,
    TAILREC_MARK = 1
} tailrec_op;

tailrec_type expr_id_tailrec(unsigned int syn_level, func * func_value, expr * value);
tailrec_type expr_tailrec(unsigned int syn_level, func * func_value, tailrec_op op, expr * value);
tailrec_type expr_list_tailrec(unsigned int syn_level, func * func_value, tailrec_op op, expr_list * list);
tailrec_type func_tailrec(unsigned int syn_level, tailrec_op op, func * value);

int func_list_tailrec(unsigned int syn_level, func_list * list);
int never_tailrec(never * nev);

#endif /* __TAILREC_H__ */


