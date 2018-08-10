/**
 * Copyright 2018 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "libmath.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

param_list * params_int_x()
{
    param_list * params = NULL;

    params = param_list_new();
    param_list_add_end(params, param_new_int(strdup("x")));

    return params;
}

param_list * params_float_x()
{
    param_list * params = NULL;

    params = param_list_new();
    param_list_add_end(params, param_new_float(strdup("x")));

    return params;
}

param_list * params_float_x_float_y()
{
    param_list * params = NULL;

    params = param_list_new();
    param_list_add_end(params, param_new_float(strdup("x")));
    param_list_add_end(params, param_new_float(strdup("y")));

    return params;
}

expr_list * params_x()
{
    expr_list * params = NULL;

    params = expr_list_new();
    expr_list_add_end(params, expr_new_id(strdup("x")));

    return params;
}

expr_list * params_x_y()
{
    expr_list * params = NULL;

    params = expr_list_new();
    expr_list_add_end(params, expr_new_id(strdup("x")));
    expr_list_add_end(params, expr_new_id(strdup("y")));

    return params;
}

func * lib_math_func_any_new(const char * name, libmath_func math_id,
                             param_list * formal, expr_list * actual, param * param_ret)
{
    char * func_id = NULL;
    expr * func_expr = NULL;
    func_body * body = NULL;
    func * func_value = NULL;

    if (param_ret->type == PARAM_INT)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_int(NULL));
    }
    else if (param_ret->type == PARAM_FLOAT)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_float(NULL));
    }
    else
    {
        printf("build_in with ret type not supported\n");
        assert(0);
    }
    body = func_body_new(NULL, NULL, func_expr);

    func_id = strdup(name);
    func_value = func_new(func_id, formal, param_ret, body);

    return func_value;
}

func * libmath_func_int_x_new(const char * name, libmath_func math_id)
{
    return lib_math_func_any_new(name, math_id, params_int_x(), params_x(),
                                 param_new_int(NULL));
}

func * libmath_func_float_x_new(const char * name, libmath_func math_id)
{
    return lib_math_func_any_new(name, math_id, params_float_x(), params_x(),
                                 param_new_float(NULL));
}

func * libmath_func_float_x_float_y_new(const char * name, libmath_func math_id)
{
    return lib_math_func_any_new(name, math_id, params_float_x_float_y(),
                                 params_x_y(), param_new_float(NULL));
}

func * libmath_func_float_x_float_y_int_new(const char * name,
                                            libmath_func math_id)
{
    return lib_math_func_any_new(name, math_id, params_float_x_float_y(),
                                 params_x_y(), param_new_int(NULL));
}

func * libmath_func_sin_new()
{
    return libmath_func_float_x_new("sin", LIB_MATH_SIN);
}

func * libmath_func_cos_new()
{
    return libmath_func_float_x_new("cos", LIB_MATH_COS);
}

func * libmath_func_tan_new()
{
    return libmath_func_float_x_new("tan", LIB_MATH_TAN);
}

func * libmath_func_exp_new()
{
    return libmath_func_float_x_new("exp", LIB_MATH_EXP);
}

func * libmath_func_log_new()
{
    return libmath_func_float_x_new("log", LIB_MATH_LOG);
}

func * libmath_func_sqrt_new()
{
    return libmath_func_float_x_new("sqrt", LIB_MATH_SQRT);
}

func * libmath_func_pow_new()
{
    return libmath_func_float_x_float_y_new("pow", LIB_MATH_POW);
}

func * libmath_func_print_int_new()
{
    return libmath_func_int_x_new("print", LIB_MATH_PRINT);
}

func * libmath_func_print_float_new()
{
    return libmath_func_float_x_new("printf", LIB_MATH_PRINTF);
}

func * libmath_func_assert_int_new()
{
    return libmath_func_int_x_new("assert", LIB_MATH_ASSERT);
}

func * libmath_func_assert_float_new()
{
    return libmath_func_float_x_float_y_int_new("assertf", LIB_MATH_ASSERTF);
}

void libmath_add_funcs(func_list * funcs)
{
    func_list_add_end(funcs, libmath_func_sin_new());
    func_list_add_end(funcs, libmath_func_cos_new());
    func_list_add_end(funcs, libmath_func_tan_new());
    func_list_add_end(funcs, libmath_func_exp_new());
    func_list_add_end(funcs, libmath_func_log_new());
    func_list_add_end(funcs, libmath_func_sqrt_new());
    func_list_add_end(funcs, libmath_func_pow_new());
    func_list_add_end(funcs, libmath_func_print_int_new());
    func_list_add_end(funcs, libmath_func_print_float_new());
    func_list_add_end(funcs, libmath_func_assert_int_new());
    func_list_add_end(funcs, libmath_func_assert_float_new());
}

const char * libmath_func_to_str(libmath_func math_id)
{
    switch (math_id)
    {
    case LIB_MATH_UNKNOWN:
        return "unknown";
    case LIB_MATH_SIN:
        return "sin";
    case LIB_MATH_COS:
        return "cos";
    case LIB_MATH_TAN:
        return "tan";
    case LIB_MATH_EXP:
        return "exp";
    case LIB_MATH_LOG:
        return "log";
    case LIB_MATH_SQRT:
        return "sqrt";
    case LIB_MATH_POW:
        return "pow";
    case LIB_MATH_PRINT:
        return "print";
    case LIB_MATH_PRINTF:
        return "printf";
    case LIB_MATH_ASSERT:
        return "assert";
    case LIB_MATH_ASSERTF:
        return "assertf";
    }
    return "unknown";
}
