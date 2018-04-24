#ifndef __TYPES_H__
#define __TYPES_H__

#include "expr.h"
#include "var.h"
#include "func.h"
#include "never.h"

typedef union token_value
{
    char char_value;
    int int_value;
    char * str_value;
    expr * expr_value;
    expr_list * expr_list_value;
    var * var_value;
    var_list * var_list_value;
    func * func_value;
    func_list * func_list_value;
    func_body * func_body_value;
    never * never_value;
} token_value;

typedef struct token
{
    int type;
    token_value val;
} token;

#define YYSTYPE token

#endif /* __TYPES_H__ */

