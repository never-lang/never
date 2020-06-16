/**
 * Copyright 2020 Slawomir Maludzinski
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
#ifndef __VMFFI_H__
#define __VMFFI_H__

#include "vm.h"
#include "bytecode.h"

typedef struct vm vm;
typedef struct bytecode bytecode;

void vm_execute_func_ffi(vm * machine, bytecode * code);
void vm_execute_func_ffi_bool(vm * machine, bytecode * code);
void vm_execute_func_ffi_int(vm * machine, bytecode * code);
void vm_execute_func_ffi_float(vm * machine, bytecode * code);
void vm_execute_func_ffi_char(vm * machine, bytecode * code);
void vm_execute_func_ffi_string(vm * machine, bytecode * code);
void vm_execute_func_ffi_void(vm * machine, bytecode * code);
void vm_execute_func_ffi_c_ptr(vm * machine, bytecode * code);
void vm_execute_func_ffi_record(vm * machine, bytecode * code);

#endif /* __VMFFI_H__ */


