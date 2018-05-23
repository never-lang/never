#ifndef __LIBMATH_H__
#define __LIBMATH_H__

#include "func.h"

typedef enum libmath_func
{
    LIB_MATH_UNKNOWN = 0,
    LIB_MATH_SIN = 1,
    LIB_MATH_COS = 2,
    LIB_MATH_TAN = 3,
    LIB_MATH_EXP = 4,
    LIB_MATH_LOG = 5,
    LIB_MATH_SQRT = 6
} libmath_func;

func * libmath_func_any_new(const char * name, libmath_func id);

func * libmath_func_sin_new();
func * libmath_func_cos_new();
func * libmath_func_tan_new();
func * libmath_func_exp_new();
func * libmath_func_log_new();
func * libmath_func_sqrt_new();

void libmath_add_funcs(func_list * funcs);

#endif /* __LIBMATH_H__ */

