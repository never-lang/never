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
#include "never.h"
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

param_list * params_long_x()
{
    param_list * params = NULL;

    params = param_list_new();
    param_list_add_end(params, param_new_long(strdup("x")));

    return params;
}

param_list * params_float_x()
{
    param_list * params = NULL;

    params = param_list_new();
    param_list_add_end(params, param_new_float(strdup("x")));

    return params;
}

param_list * params_double_x()
{
    param_list * params = NULL;

    params = param_list_new();
    param_list_add_end(params, param_new_double(strdup("x")));

    return params;
}

param_list * params_bool_x()
{
    param_list * params = NULL;
    
    params = param_list_new();
    param_list_add_end(params, param_new_bool(strdup("x")));
    
    return params;
}

param_list * params_char_x()
{
    param_list * params = NULL;
    
    params = param_list_new();
    param_list_add_end(params, param_new_char(strdup("x")));
    
    return params;
}

param_list * params_string_x()
{
    param_list * params = NULL;
    
    params = param_list_new();
    param_list_add_end(params, param_new_string(strdup("x")));
    
    return params;
}

param_list * params_c_ptr_x()
{
    param_list * params = NULL;
    
    params = param_list_new();
    param_list_add_end(params, param_new_c_ptr(strdup("x")));
    
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

func * lib_math_func_any_new(libmath_func math_id, param_list * formal,
                             expr_list * actual, param * param_ret)
{
    char * func_name = NULL;
    expr * func_expr = NULL;
    func_decl * decl = NULL;
    func * func_value = NULL;
    func_body * body = NULL;

    if (param_ret->type == PARAM_BOOL)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_bool(NULL));
    }
    else if (param_ret->type == PARAM_INT)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_int(NULL));
    }
    else if (param_ret->type == PARAM_LONG)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_long(NULL));
    }
    else if (param_ret->type == PARAM_FLOAT)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_float(NULL));
    }
    else if (param_ret->type == PARAM_DOUBLE)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_double(NULL));
    }
    else if (param_ret->type == PARAM_CHAR)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_char(NULL));
    }
    else if (param_ret->type == PARAM_STRING)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_string(NULL));
    }
    else if (param_ret->type == PARAM_C_PTR)
    {
        func_expr = expr_new_build_in(math_id, actual, param_new_c_ptr(NULL));
    }
    else
    {
        printf("build_in with ret type not supported\n");
        assert(0);
    }
    body = func_body_new_expr(NULL, NULL, func_expr);

    func_name = strdup(libmath_func_to_str(math_id));
    decl = func_decl_new(func_name, formal, param_ret);
    func_value = func_new(decl, body);

    return func_value;
}

func * libmath_func_bool_x_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_bool_x(), params_x(),
                                 param_new_bool(NULL));
}

func * libmath_func_int_x_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_int_x(), params_x(),
                                 param_new_int(NULL));
}

func * libmath_func_long_x_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_long_x(), params_x(),
                                 param_new_long(NULL));
}

func * libmath_func_float_x_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_float_x(), params_x(),
                                 param_new_float(NULL));
}

func * libmath_func_double_x_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_double_x(), params_x(),
                                 param_new_double(NULL));
}

func * libmath_func_float_x_float_y_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_float_x_float_y(),
                                 params_x_y(), param_new_float(NULL));
}

func * libmath_func_float_x_float_y_int_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_float_x_float_y(),
                                 params_x_y(), param_new_int(NULL));
}

func * libmath_func_char_x_int_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_char_x(), params_x(),
                                 param_new_int(NULL));
}

func * libmath_func_int_x_char_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_int_x(), params_x(),
                                 param_new_char(NULL));
}

func * libmath_func_void_int_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, NULL, NULL,
                                 param_new_int(NULL));
}

func * libmath_func_int_x_string_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_int_x(), params_x(),
                                 param_new_string(NULL));
}

func * libmath_func_float_x_string_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_float_x(), params_x(),
                                 param_new_string(NULL));
}

func * libmath_func_char_x_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_char_x(), params_x(),
                                 param_new_char(NULL));
}

func * libmath_func_string_x_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_string_x(), params_x(),
                                 param_new_string(NULL));
}

func * libmath_func_string_x_int_new(libmath_func math_id)
{
    return lib_math_func_any_new(math_id, params_string_x(), params_x(),
                                 param_new_int(NULL));
}

func * libmath_func_sin_new()
{
    return libmath_func_float_x_new(LIB_MATH_SIN);
}

func * libmath_func_cos_new()
{
    return libmath_func_float_x_new(LIB_MATH_COS);
}

func * libmath_func_tan_new()
{
    return libmath_func_float_x_new(LIB_MATH_TAN);
}

func * libmath_func_exp_new()
{
    return libmath_func_float_x_new(LIB_MATH_EXP);
}

func * libmath_func_log_new()
{
    return libmath_func_float_x_new(LIB_MATH_LOG);
}

func * libmath_func_sqrt_new()
{
    return libmath_func_float_x_new(LIB_MATH_SQRT);
}

func * libmath_func_pow_new()
{
    return libmath_func_float_x_float_y_new(LIB_MATH_POW);
}

func * libmath_func_str_int_new()
{
    return libmath_func_int_x_string_new(LIB_MATH_STR);
}

func * libmath_func_str_float_new()
{
    return libmath_func_float_x_string_new(LIB_MATH_STRF);
}

func * libmath_func_ord_new()
{
    return libmath_func_char_x_int_new(LIB_MATH_ORD);
}

func * libmath_func_chr_new()
{
    return libmath_func_int_x_char_new(LIB_MATH_CHR);
}

func * libmath_func_read_new()
{
    return libmath_func_void_int_new(LIB_MATH_READ);
}

func * libmath_func_print_bool_new()
{
    return libmath_func_bool_x_new(LIB_MATH_PRINTB);
}

func * libmath_func_print_int_new()
{
    return libmath_func_int_x_new(LIB_MATH_PRINT);
}

func * libmath_func_print_long_new()
{
    return libmath_func_long_x_new(LIB_MATH_PRINTL);
}

func * libmath_func_print_float_new()
{
    return libmath_func_float_x_new(LIB_MATH_PRINTF);
}

func * libmath_func_print_double_new()
{
    return libmath_func_double_x_new(LIB_MATH_PRINTD);
}

func * libmath_func_print_char_new()
{
    return libmath_func_char_x_new(LIB_MATH_PRINTC);
}

func * libmath_func_print_string_new()
{
    return libmath_func_string_x_new(LIB_MATH_PRINTS);
}

func * libmath_func_length_new()
{
    return libmath_func_string_x_int_new(LIB_MATH_LENGTH);
}

func * libmath_func_assert_int_new()
{
    return libmath_func_bool_x_new(LIB_MATH_ASSERT);
}

func * libmath_func_assert_float_new()
{
    return libmath_func_float_x_float_y_int_new(LIB_MATH_ASSERTF);
}

func * libmath_func_c_int_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_INT_PTR, params_int_x(), params_x(),
                                 param_new_c_ptr(NULL));
}

func * libmath_func_c_long_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_LONG_PTR, params_long_x(), params_x(),
                                 param_new_c_ptr(NULL));
}

func * libmath_func_c_float_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_FLOAT_PTR, params_float_x(), params_x(),
                                 param_new_c_ptr(NULL));
}

func * libmath_func_c_double_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_DOUBLE_PTR, params_double_x(), params_x(),
                                 param_new_c_ptr(NULL));
}

func * libmath_func_c_bool_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_BOOL_PTR, params_bool_x(), params_x(),
                                 param_new_c_ptr(NULL));
}

func * libmath_func_c_char_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_CHAR_PTR, params_char_x(), params_x(),
                                 param_new_c_ptr(NULL));
}

func * libmath_func_c_string_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_STRING_PTR, params_string_x(), params_x(),
                                 param_new_c_ptr(NULL));
}

func * libmath_func_c_ptr_ptr_new()
{
    return lib_math_func_any_new(LIB_MATH_C_PTR_PTR, params_c_ptr_x(), params_x(),
                                 param_new_c_ptr(NULL));
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
    func_list_add_end(funcs, libmath_func_str_int_new());
    func_list_add_end(funcs, libmath_func_str_float_new());
    func_list_add_end(funcs, libmath_func_ord_new());
    func_list_add_end(funcs, libmath_func_chr_new());
    func_list_add_end(funcs, libmath_func_read_new());
    func_list_add_end(funcs, libmath_func_print_bool_new());
    func_list_add_end(funcs, libmath_func_print_int_new());
    func_list_add_end(funcs, libmath_func_print_long_new());
    func_list_add_end(funcs, libmath_func_print_float_new());
    func_list_add_end(funcs, libmath_func_print_double_new());
    func_list_add_end(funcs, libmath_func_print_char_new());
    func_list_add_end(funcs, libmath_func_print_string_new());
    func_list_add_end(funcs, libmath_func_length_new());
    func_list_add_end(funcs, libmath_func_assert_int_new());
    func_list_add_end(funcs, libmath_func_assert_float_new());
    func_list_add_end(funcs, libmath_func_c_int_ptr_new());
    func_list_add_end(funcs, libmath_func_c_long_ptr_new());
    func_list_add_end(funcs, libmath_func_c_float_ptr_new());
    func_list_add_end(funcs, libmath_func_c_double_ptr_new());
    func_list_add_end(funcs, libmath_func_c_bool_ptr_new());
    func_list_add_end(funcs, libmath_func_c_char_ptr_new());
    func_list_add_end(funcs, libmath_func_c_string_ptr_new());
    func_list_add_end(funcs, libmath_func_c_ptr_ptr_new());            
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
    case LIB_MATH_STR:
        return "str";
    case LIB_MATH_STRF:
        return "strf";
    case LIB_MATH_ORD:
        return "ord";
    case LIB_MATH_CHR:
        return "chr";
    case LIB_MATH_READ:
        return "read";
    case LIB_MATH_PRINT:
        return "print";
    case LIB_MATH_PRINTL:
        return "printl";
    case LIB_MATH_PRINTB:
        return "printb";
    case LIB_MATH_PRINTF:
        return "printf";
    case LIB_MATH_PRINTD:
        return "printd";
    case LIB_MATH_PRINTC:
        return "printc";
    case LIB_MATH_PRINTS:
        return "prints";
    case LIB_MATH_LENGTH:
        return "length";
    case LIB_MATH_ASSERT:
        return "assert";
    case LIB_MATH_ASSERTF:
        return "assertf";
    case LIB_MATH_C_INT_PTR:
        return "c_int_ptr";
    case LIB_MATH_C_LONG_PTR:
        return "c_long_ptr";
    case LIB_MATH_C_FLOAT_PTR:
        return "c_float_ptr";
    case LIB_MATH_C_DOUBLE_PTR:
        return "c_double_ptr";
    case LIB_MATH_C_BOOL_PTR:
        return "c_bool_ptr";
    case LIB_MATH_C_CHAR_PTR:
        return "c_char_ptr";
    case LIB_MATH_C_STRING_PTR:
        return "c_string_ptr";
    case LIB_MATH_C_PTR_PTR:
        return "c_ptr_ptr";
    }
    return "unknown";
}

void libmath_add_funcs_module_decl(module_decl * module_value)
{
    if (module_value->nev && module_value->nev->funcs)
    {
        libmath_add_funcs(module_value->nev->funcs);
    }
}
