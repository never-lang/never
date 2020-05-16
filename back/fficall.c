/**
 * Copyright 2018-2020 Slawomir Maludzinski
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
#ifndef NO_FFI

#include "fficall.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dlfcn.h>

ffi_decl * ffi_decl_new(unsigned int count)
{
    ffi_decl * value = (ffi_decl *)malloc(sizeof(ffi_decl));
    
    value->count = count;
    value->param_types = (ffi_type **)malloc(count * sizeof(ffi_type *));
    value->param_values = (void **)malloc(count * sizeof(void *));
    value->ret_type = NULL;
    value->ret_void_value = NULL;

    memset(value->param_types, 0, count * sizeof(ffi_type*));
    memset(value->param_values, 0, count * sizeof(void *));

    return value;
}

void ffi_decl_delete_ffi_type(ffi_type * type)
{
    unsigned int i = 0;

    while (type->elements[i] != NULL)
    {
        if (type->elements[i]->type == FFI_TYPE_STRUCT)
        {
            ffi_decl_delete_ffi_type(type->elements[i]);
        }
        i++;
    }

    free(type->elements);
    free(type);
}

void ffi_decl_delete(ffi_decl * value)
{
    unsigned int i = 0;

    for (i = 0; i < value->count; i++)
    {
        if (value->param_types[i] != NULL &&
            value->param_types[i]->type == FFI_TYPE_STRUCT)
        {
            ffi_decl_delete_ffi_type(value->param_types[i]);
            free(value->param_values[i]);
        }
    }

    if (value->param_types != NULL)
    {
        free(value->param_types);
    }
    if (value->param_values != NULL)
    {
        free(value->param_values);
    }
    if (value->ret_type != NULL &&
        value->ret_type->type == FFI_TYPE_STRUCT)
    {
        free(value->ret_void_value);
        ffi_decl_delete_ffi_type(value->ret_type);
    }

    free(value);
}

int ffi_decl_set_param_type(ffi_decl * decl, unsigned int index, ffi_type * param_type)
{
    assert(index < decl->count);
    
    decl->param_types[index] = param_type;

    return FFI_SUCC;
}

int ffi_decl_set_param_value(ffi_decl * decl, unsigned int index, void * param_value)
{
    assert(index < decl->count);
    
    decl->param_values[index] = param_value;

    return FFI_SUCC;
}

int ffi_decl_set_ret_type(ffi_decl * decl, ffi_type * ret_type)
{
    decl->ret_type = ret_type;

    return FFI_SUCC;
}

int ffi_decl_prepare(ffi_decl * decl)
{
    ffi_status status;

    status = ffi_prep_cif(&decl->cif, FFI_DEFAULT_ABI, decl->count,
                          decl->ret_type, decl->param_types);
    if (status != FFI_OK)
    {
        fprintf(stderr, "ffi_prep_cif returned an error %d\n", status);
        return FFI_FAIL;
    }

    /* allocate memory for return */
    if (decl->ret_type->type == FFI_TYPE_STRUCT)
    {
        decl->ret_void_value = (void *)malloc(decl->ret_type->size);
    }

    return FFI_SUCC;
}

void * ffi_decl_get_handle(const char * libname)
{
    void * handle = NULL;

    /* open for main program */    
    if (strcmp(libname, "host") == 0)
    {
        libname = NULL;
    }

    handle = dlopen(libname, RTLD_LAZY);
    if (handle == NULL)
    {
        fprintf(stderr, "cannot open library %s\n", libname);
        return NULL;
    }
    
    return handle;
}

void ffi_decl_close_handle(void * handle)
{
    if (handle != NULL)
    {
        dlclose(handle);
    }
}

int ffi_decl_call(ffi_decl * decl, char * fname, void * handle)
{
    void (* func)(void) = NULL;

    func = dlsym(handle, fname);
    if (func == NULL)
    {
        fprintf(stderr, "cannot obtain address of a symbol %s\n", fname);
        return FFI_FAIL;
    }

    if (decl->ret_type->type == FFI_TYPE_STRUCT)
    {
        ffi_call(&decl->cif, FFI_FN(func), decl->ret_void_value, decl->param_values);
    }
    else
    {
        ffi_call(&decl->cif, FFI_FN(func), &decl->ret_void_value, decl->param_values);
    }

    return FFI_SUCC;
}

/* FFI test functions to self test never-lang */

char test_char(char c)
{
    return ((c - 'A' + 1) % ('Z' - 'A' + 1)) + 'A';
}

int test_print_str(const char * str)
{
    printf("test print str %s", str);
    
    return 0;
}

char * test_conc_str(const char * a, const char * b)
{
    static char c[20];
    
    sprintf(c, "%s%s", a, b);
    
    return c;
}

char * test_conc_int_str(int d, const char * s)
{
    static char c[20];
    
    sprintf(c, "%d:%s", d, s);

    return c;
}

test_Point test_rect(test_Rect r)
{
    test_Point rec = { 110, 120 };

    printf("test rect [ %d %d ] [ %d %d ] [ %d %d ]\n", r.a.x, r.a.y, r.b.x, r.b.y, r.c.x, r.c.y);

    return rec;
}

char * test_void()
{
    return strdup("one two three");
}

void test_void_call(char * str)
{
    printf("%s\n", str);
    free(str);
}

int test_types_get_int()
{
    return 10;
}

float test_types_get_float()
{
    return 10.0;
}

char test_types_get_char()
{
    return 'C';
}

char * test_types_get_string()
{
    return "CCC";
}

test_Point p1 = { 10, 20 };

test_Point * test_types_get_ptr()
{
    return &p1;
}

void test_types_call(int i, float f, char c, char * str, test_Point * ptr, test_Types rec)
{
    assert(i == 10);
    assert(f == 10.0);
    assert(c == 'A');
    assert(strcmp(str, "AAA") == 0);
    assert(ptr->x == 10);
    assert(ptr->y == 20);

    assert(rec.i == 20);
    assert(rec.f == 20.0);
    assert(rec.c == 'B');
    assert(strcmp(rec.str, "BBB") == 0);
    assert(rec.ptr->x == 10);
    assert(rec.ptr->y == 20);
    assert(rec.rec.x == 30);
    assert(rec.rec.y == 40);
}

test_Types test_types_get_rec()
{
    test_Types ret;

    ret.i = 10;
    ret.f = 10.0;
    ret.c = 'A';
    ret.str = "AAA";
    ret.ptr = &p1;
    ret.rec.x = 20;
    ret.rec.y = 30;

    return ret;
}

#endif /* NO_FFI */
