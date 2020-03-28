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
    LIB_MATH_STR,
    LIB_MATH_STRF,
    LIB_MATH_ORD,
    LIB_MATH_CHR,
    LIB_MATH_READ,
    LIB_MATH_PRINT,
    LIB_MATH_PRINTB,
    LIB_MATH_PRINTF,
    LIB_MATH_PRINTC,
    LIB_MATH_PRINTS,
    LIB_MATH_LENGTH,
    LIB_MATH_ASSERT,
    LIB_MATH_ASSERTF
} libmath_func;

param_list * params_bool_x();
param_list * params_int_x();
param_list * params_float_x();
param_list * params_float_x_float_y();
param_list * params_char_x();
param_list * params_string_x();
expr_list * params_x();
expr_list * params_x_y();

func * lib_math_func_any_new(libmath_func math_id, param_list * formal,
                             expr_list * actual, param * param_ret);
func * libmath_func_bool_x_new(libmath_func math_id);
func * libmath_func_int_x_new(libmath_func math_id);
func * libmath_func_float_x_new(libmath_func math_id);
func * libmath_func_float_x_float_y_new(libmath_func math_id);
func * libmath_func_float_x_float_y_int_new(libmath_func math_id);
func * libmath_func_char_x_int_new(libmath_func math_id);
func * libmath_func_int_x_char_new(libmath_func math_id);
func * libmath_func_void_int_new(libmath_func math_id);
func * libmath_func_string_x_new(libmath_func math_id);
func * libmath_func_string_x_int_new(libmath_func math_id);

func * libmath_func_sin_new();
func * libmath_func_cos_new();
func * libmath_func_tan_new();
func * libmath_func_exp_new();
func * libmath_func_log_new();
func * libmath_func_sqrt_new();
func * libmath_func_pow_new();
func * libmath_func_str_int_new();
func * libmath_func_str_float_new();
func * libmath_func_ord_new();
func * libmath_func_chr_new();
func * libmath_func_read_new();
func * libmath_func_print_bool_new();
func * libmath_func_print_int_new();
func * libmath_func_print_float_new();
func * libmath_func_print_char_new();
func * libmath_func_print_string_new();
func * libmath_func_length_new();
func * libmath_func_assert_int_new();
func * libmath_func_assert_float_new();

void libmath_add_funcs(func_list * funcs);

const char * libmath_func_to_str(libmath_func math_id);

#endif /* __LIBMATH_H__ */
