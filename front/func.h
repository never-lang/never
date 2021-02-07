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
#ifndef __FUNC_H__
#define __FUNC_H__

#include "expr.h"
#include "freevar.h"
#include "symtab.h"
#include "param.h"
#include "bind.h"
#include "except.h"
#include <stdbool.h>

typedef struct func_body func_body;
typedef struct func_decl func_decl;
typedef struct func_except func_except;

typedef enum func_type
{
    FUNC_TYPE_UNKNOWN = 0,
    FUNC_TYPE_NATIVE = 1,
    FUNC_TYPE_FFI = 2,
} func_type;

typedef struct func
{
    func_type type;
    int entry;
    int index;
    func_decl * decl;
    symtab * stab;
    union
    {
        struct
        {
            func_body * body;
            func_except * except;
            struct freevar_list * freevars;
        };
        char * libname;
    };
    unsigned int addr;
    unsigned int line_no;
} func;

typedef struct func_decl
{
    char * id;
    param_list * params;
    param * ret;
    bool checked;
} func_decl;

typedef struct func_body
{
    expr * exprs;
} func_body;

typedef struct func_except
{
    except_list * list; 
    except * all;
} func_except;

func * func_new(func_decl * decl, func_body * body);
func * func_new_except(func_decl * decl, func_body * body, func_except * except);
func * func_new_ffi(char * libname, func_decl * decl);
void func_delete(func * value);

void func_print(func * value);

func_decl * func_decl_new(char * id, param_list * params, param * ret);
void func_decl_delete(func_decl * value);

func_body * func_body_new(seq * seq_value);
func_body * func_body_new_expr(expr * exprs);
void func_body_delete(func_body * body);

func_except * func_except_new(except * all, except_list * list);
void func_except_delete(func_except * value);

#endif /* __FUNC_H__ */

