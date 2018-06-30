#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "expr.h"
#include "var.h"

typedef enum array_type
{
    ARRAY_UNKNOWN = 0,
    ARRAY_INIT = 1,
    ARRAY_SUB = 2,
    ARRAY_DIMS = 3
} array_type;

typedef struct array
{
    array_type type;
    expr_list * elements;
    expr_list * dims;
    var * ret;
    unsigned int line_no;
} array;

array * array_new(expr_list * elements, var * ret);
array * array_new_sub(expr_list * elements);
array * array_new_dims(expr_list * dims, var * ret);

void array_delete(array * value);

#endif /* __ARRAY_H__ */


