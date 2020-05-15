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
    
    return value;
}

void ffi_decl_delete(ffi_decl * value)
{
    if (value->param_types != NULL)
    {
        free(value->param_types);
    }
    if (value->param_values != NULL)
    {
        free(value->param_values);
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

    ffi_call(&decl->cif, FFI_FN(func), &decl->ret_void_value, decl->param_values);

    return FFI_SUCC;
}

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

#endif /* NO_FFI */

