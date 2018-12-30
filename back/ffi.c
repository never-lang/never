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
#include "ffi.h"
#include <stdlib.h>
#include <assert.h>

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

    return 0;
}

int ffi_decl_set_param_value(ffi_decl * decl, unsigned int index, void * param_value)
{
    assert(index < decl->count);
    
    decl->param_values[index] = param_value;

    return 0;
}

int ffi_decl_set_ret(ffi_decl * decl, ffi_type ret)
{
    decl->ret = ret;

    return 0;
}

int ffi_decl_prepare(ffi_decl * decl)
{
    ffi_status status;

    status = ffi_prep_cif(&decl->cif, FFI_DEFAULT_ABI, decl->count,
                          &decl->ret, decl->param_types);
    if (status != FFI_OK)
    {
        return 1;
    }

    return 0;
}

int ffi_decl_call(ffi_decl * decl)
{
    ffi_call(&decl->cif, FFI_FN(decl->func), &decl->ret, decl->param_values);

    return 0;
}


