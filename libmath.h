#ifndef __LIBMATH_H__
#define __LIBMATH_H__

#include "func.h"

typedef enum libmath_func
{
    LIB_MATH_UNKNOWN = 0,
    LIB_MATH_SIN,
    LIB_MATH_COS,
    LIB_MATH_TAN,
    LIB_MATH_EXP,
    LIB_MATH_LOG,
    LIB_MATH_SQRT,
    LIB_MATH_POW,
    LIB_MATH_PRINT,
    LIB_MATH_PRINTF,
    LIB_MATH_ASSERT,
    LIB_MATH_ASSERTF
} libmath_func;

var_list * vars_int_x();
var_list * vars_float_x();
var_list * vars_float_x_float_y();
expr_list * params_x();
expr_list * params_x_y();

func * lib_math_func_any_new(const char * name, libmath_func math_id,
                             var_list * vars, expr_list * params, var * var_ret);
func * libmath_func_int_x_new(const char * name, libmath_func math_id);
func * libmath_func_float_x_new(const char * name, libmath_func math_id);
func * libmath_func_float_x_float_y_new(const char * name, libmath_func math_id);
func * libmath_func_float_x_float_y_int_new(const char * name, libmath_func math_id);

func * libmath_func_sin_new();
func * libmath_func_cos_new();
func * libmath_func_tan_new();
func * libmath_func_exp_new();
func * libmath_func_log_new();
func * libmath_func_sqrt_new();
func * libmath_func_print_int_new();
func * libmath_func_print_float_new();
func * libmath_func_assert_int_new();
func * libmath_func_assert_float_new();

void libmath_add_funcs(func_list * funcs);

const char * libmath_func_to_str(libmath_func math_id);

#endif /* __LIBMATH_H__ */
