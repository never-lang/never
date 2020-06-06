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
#ifndef __VM_H__
#define __VM_H__

#include "program.h"

typedef struct gc gc;
typedef struct bytecode bytecode;

typedef enum vm_state
{
    VM_HALT = 0,
    VM_RUNNING = 1,
    VM_EXCEPTION = 2,
    VM_ERROR = 3
} vm_state;

typedef enum except_no
{
    EXCEPT_NO_UNKNOWN = 0,
    EXCEPT_NO_DIVISION = 1,
    EXCEPT_NO_ARR_SIZE = 2,
    EXCEPT_NO_INDEX_OOB = 3,
    EXCEPT_NO_INVALID = 4,
    EXCEPT_NO_OVERFLOW = 5,
    EXCEPT_NO_UNDERFLOW = 6,
    EXCEPT_NO_INEXACT = 7,
    EXCEPT_NIL_POINTER = 8,
    EXCEPT_FFI_FAIL = 9,
} except_no;

typedef struct vm
{
    stack_ptr sp;
    stack_ptr fp;
    stack_ptr pp;
    mem_ptr gp;
    ip_ptr ip;

    int stack_size;
    struct gc_stack * stack;
    struct gc * collector;
    struct dlcache * dlib_cache;

    except_no exception;
    unsigned int line_no;
    vm_state running;
    unsigned int initialized;

    program * prog;
} vm;

vm * vm_new(unsigned int mem_size, unsigned int stack_size);
void vm_delete(vm * machine);

void vm_check_stack(vm * machine);
int vm_execute(vm * machine, program * prog, object * result);

void vm_print_stack_trace(vm * machine);
void vm_print(vm * machine, const char * msg);

#endif /* __VM_H__ */
