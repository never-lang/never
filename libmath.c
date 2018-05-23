#include <stdio.h>
#include <string.h>
#include "libmath.h"

func * libmath_func_any_new(const char * name, libmath_func math_id)
{
    char * func_id = strdup(name);
    
    var * var_x = var_new_float(strdup("x"));
    var_list * vars = var_list_new();
    var * var_ret = var_new_float(NULL);
    
    expr * param = expr_new_id(strdup("x"));
    expr * func_expr = expr_new_build_in(math_id, param);
    
    func_body * body = func_body_new(NULL, func_expr);
    func * func_value = NULL;
    
    var_list_add_end(vars, var_x);
    
    func_value = func_new(func_id, vars, var_ret, body);
    
    return func_value;
}

func * libmath_func_sin_new()
{
    return libmath_func_any_new("sin", LIB_MATH_SIN);
}

func * libmath_func_cos_new()
{
    return libmath_func_any_new("cos", LIB_MATH_COS);
}

func * libmath_func_tan_new()
{
    return libmath_func_any_new("tan", LIB_MATH_TAN);
}

func * libmath_func_exp_new()
{
    return libmath_func_any_new("exp", LIB_MATH_EXP);
}

func * libmath_func_log_new()
{
    return libmath_func_any_new("log", LIB_MATH_LOG);
}

func * libmath_func_sqrt_new()
{
    return libmath_func_any_new("sqrt", LIB_MATH_SQRT);
}

void libmath_add_funcs(func_list * funcs)
{
    func_list_add_end(funcs, libmath_func_sin_new());
    func_list_add_end(funcs, libmath_func_cos_new());
    func_list_add_end(funcs, libmath_func_tan_new());
    func_list_add_end(funcs, libmath_func_exp_new());
    func_list_add_end(funcs, libmath_func_log_new());
    func_list_add_end(funcs, libmath_func_sqrt_new());
}


