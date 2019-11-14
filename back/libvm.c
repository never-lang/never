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
#include "libvm.h"
#include "gc.h"
#include "libmath.h"
#include "utils.h"
#include "strutil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fenv.h>
#include <assert.h>

void libvm_execute_build_in(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = 0;

    feclearexcept(FE_ALL_EXCEPT);

    switch (code->build_in.id)
    {
    case LIB_MATH_UNKNOWN:
        fprintf(stderr, "unknown math function\n");
        assert(0);
        break;
    case LIB_MATH_SIN:
    {
        float value = 0;
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        value = sinf(x);
        addr = gc_alloc_float(machine->collector, value);
    }
    break;
    case LIB_MATH_COS:
    {
        float value = 0;
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        value = cosf(x);
        addr = gc_alloc_float(machine->collector, value);
    }
    break;
    case LIB_MATH_TAN:
    {
        float value = 0;
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        value = tanf(x);
        addr = gc_alloc_float(machine->collector, value);
    }
    break;
    case LIB_MATH_EXP:
    {
        float value = 0;
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        value = expf(x);
        addr = gc_alloc_float(machine->collector, value);
    }
    break;
    case LIB_MATH_LOG:
    {
        float value = 0;
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        value = logf(x);
        addr = gc_alloc_float(machine->collector, value);
    }
    break;
    case LIB_MATH_SQRT:
    {
        float value = 0;
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        value = sqrtf(x);
        addr = gc_alloc_float(machine->collector, value);
    }
    break;
    case LIB_MATH_POW:
    {
        float value = 0;
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        float y = gc_get_float(machine->collector,
                               machine->stack[machine->sp - 1].addr);
        value = powf(x, y);
        addr = gc_alloc_float(machine->collector, value);
        machine->sp--;
    }
    break;
    case LIB_MATH_STR:
    {
        int x = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
        mem_ptr str_x = gc_alloc_string_take(machine->collector, string_int(x));        
        addr = gc_alloc_string_ref(machine->collector, str_x);
    }
    break;
    case LIB_MATH_STRF:
    {
        float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        mem_ptr str_x = gc_alloc_string_take(machine->collector, string_float(x));
        addr = gc_alloc_string_ref(machine->collector, str_x);
    }
    break;
    case LIB_MATH_PRINT:
    {
        int x =
            gc_get_int(machine->collector, machine->stack[machine->sp].addr);
        string_print_int(x);
        addr = gc_alloc_int(machine->collector, x);
    }
    break;
    case LIB_MATH_PRINTF:
    {
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        string_print_float(x);
        addr = gc_alloc_float(machine->collector, x);
    }
    break;
    case LIB_MATH_PRINTC:
    {
        char x = gc_get_char(machine->collector, machine->stack[machine->sp].addr);
        string_print_char(x);
        addr = gc_alloc_char(machine->collector, x);
    }
    break;
    case LIB_MATH_PRINTS:
    {
        mem_ptr str_x = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr);
        if (str_x == nil_ptr)
        {
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NIL_POINTER;
            return;
        }
        char * x = gc_get_string(machine->collector, str_x);
        string_print(x);
        addr = gc_alloc_string_ref(machine->collector, str_x);
    }
    break;
    case LIB_MATH_LENGTH:
    {
        mem_ptr str_x = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr);
        if (str_x == nil_ptr)
        {
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NIL_POINTER;
            return;
        }

        char * x = gc_get_string(machine->collector, str_x);
        if (x == 0)
        {
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NIL_POINTER;
            return;
        }
        
        int length = (int)strlen(x);
        addr = gc_alloc_int(machine->collector, length);
    }
    break;
    case LIB_MATH_ASSERT:
    {
        int x =
            gc_get_int(machine->collector, machine->stack[machine->sp].addr);
        if (x == 0)
        {
            print_error_msg(machine->line_no, "assert failed\n");
            machine->running = VM_ERROR;
            return;
        }
        addr = gc_alloc_int(machine->collector, 1);
    }
    break;
    case LIB_MATH_ASSERTF:
    {
        float x =
            gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        float delta = gc_get_float(machine->collector,
                                   machine->stack[machine->sp - 1].addr);
        if (-delta > x || x > delta)
        {
            print_error_msg(machine->line_no, "assert failed\n");
            machine->running = VM_ERROR;
            return;
        }
        addr = gc_alloc_int(machine->collector, 1);
        machine->sp--;
    }
    break;
    default:
        fprintf(stderr, "unknown build in function id %d\n", code->build_in.id);
        assert(0);
    }

    if (fetestexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW))
    {
        if (fetestexcept(FE_DIVBYZERO))
        {
            machine->exception = EXCEPT_NO_DIVISION;
        }
        else if (fetestexcept(FE_INVALID))
        {
            machine->exception = EXCEPT_NO_INVALID;
        }
        else if (fetestexcept(FE_OVERFLOW))
        {
            machine->exception = EXCEPT_NO_OVERFLOW;
        }
        else if (fetestexcept(FE_UNDERFLOW))
        {
            machine->exception = EXCEPT_NO_UNDERFLOW;
        }

        print_error_msg(machine->line_no,
                        "an error occurred in build in function %s %s\n",
                        libmath_func_to_str(code->build_in.id),
                        except_to_str(machine->exception));

        machine->running = VM_EXCEPTION;
        return;
    }

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}
