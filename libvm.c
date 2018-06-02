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
    float value = 0;
    float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);

    feclearexcept(FE_ALL_EXCEPT);

    switch (code->build_in.id)
    {
        case LIB_MATH_UNKNOWN:
            fprintf(stderr, "unknown math function\n");
            assert(0);
        break;
        case LIB_MATH_SIN:
            value = sinf(x);
        break;
        case LIB_MATH_COS:
            value = cosf(x);
        break;
        case LIB_MATH_TAN:
            value = tanf(x);
        break;
        case LIB_MATH_EXP:
            value = expf(x);
        break;
        case LIB_MATH_LOG:
            value = logf(x);
        break;
        case LIB_MATH_SQRT:
            value = sqrtf(x);
        break;
        case LIB_MATH_POW:
        {
            float y = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
            machine->sp--;
            value = powf(x, y);
        }
        break;
        case LIB_MATH_PRINTLN:
            printf("%.2f\n", x);
            value = x;
        break;
        case LIB_MATH_ASSERT:
            value = x;
            if (x == 0)
            {
                print_error_msg(machine->line_no, "assert failed\n");
                machine->running = VM_ERROR;
                return;
            }
        break;
        case LIB_MATH_ASSERTF:
        {
            float delta = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
            machine->sp--;
            value = x;
            if (-delta > x || x > delta)
            {
                print_error_msg(machine->line_no, "assert failed\n");
                machine->running = VM_ERROR;
                return;
            }
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

    mem_ptr addr = gc_alloc_float(machine->collector, value);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}


