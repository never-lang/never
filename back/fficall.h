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
#ifndef __FFICALL_H__
#define __FFICALL_H__

#ifndef NO_FFI

#include <ffi.h>

#define FFI_SUCC 0
#define FFI_FAIL 1

typedef struct ffi_decl {
    unsigned int count;
    ffi_type ** param_types;
    void ** param_values;
    ffi_type * ret_type;
    union
    {
        void * ret_void_value;
        int ret_int_value;
        float ret_float_value;
        char ret_char_value;
        char * ret_string_value;
    };
    ffi_cif cif;
} ffi_decl;

ffi_decl * ffi_decl_new(unsigned int count);
void ffi_decl_delete(ffi_decl * value);

int ffi_decl_set_param_type(ffi_decl * decl, unsigned int index, ffi_type * param_type);
int ffi_decl_set_param_value(ffi_decl * decl, unsigned int index, void * param_value);
int ffi_decl_set_ret_type(ffi_decl * decl , ffi_type * ret_type);

void * ffi_decl_get_handle(const char * libname);
void ffi_decl_close_handle(void * handle);

int ffi_decl_prepare(ffi_decl * decl);
int ffi_decl_call(ffi_decl * decl, char * fname, void * handle);

char test_char(char c);
int test_print_str(const char * str);

char * test_conc_str(const char * a, const char * b);
char * test_conc_int_str(int d, const char * s);

#endif

#endif /* __FFICALL_H__ */

