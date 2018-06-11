#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <assert.h>
#include "libvm.h"
#include "libmath.h"
#include "gc.h"
#include "utils.h"

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
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            value = sinf(x);
            addr = gc_alloc_float(machine->collector, value);
        }
        break;
        case LIB_MATH_COS:
        {
            float value = 0;
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            value = cosf(x);
            addr = gc_alloc_float(machine->collector, value);
        }
        break;
        case LIB_MATH_TAN:
        {
            float value = 0;
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            value = tanf(x);
            addr = gc_alloc_float(machine->collector, value);
        }
        break;
        case LIB_MATH_EXP:
        {
            float value = 0;
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            value = expf(x);
            addr = gc_alloc_float(machine->collector, value);
        }
        break;
        case LIB_MATH_LOG:
        {
            float value = 0;
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            value = logf(x);
            addr = gc_alloc_float(machine->collector, value);
        }
        break;
        case LIB_MATH_SQRT:
        {
            float value = 0;
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            value = sqrtf(x);
            addr = gc_alloc_float(machine->collector, value);
        }
        break;
        case LIB_MATH_POW:
        {
            float value = 0;
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            float y = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
            value = powf(x, y);
            addr = gc_alloc_float(machine->collector, value);
            machine->sp--;
        }
        break;
        case LIB_MATH_PRINT:
        {
            int x = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
            printf("%d\n", x);
            addr = gc_alloc_int(machine->collector, x);
        }
        break;
        case LIB_MATH_PRINTF:
        {
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            printf("%.2f\n", x);
            addr = gc_alloc_float(machine->collector, x);
        }
        break;
        case LIB_MATH_ASSERT:
        {
            int x = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
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
            float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
            float delta = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
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
    }

    if (fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW))
    {
        print_error_msg(machine->line_no,
                        "an error occurred in build in function %s\n",
                        libmath_func_to_str(code->build_in.id));
        machine->running = VM_ERROR;
        return;
    }

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}


