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

#include "nev.h"
#include "fficall.h"
#include <stdio.h>
#include <assert.h>

void test_zero()
{
    assert(test_char('A') == 'B');
    assert(test_char('B') == 'C');
    assert(test_char('Z') == 'A');
}

void test_one()
{
    ffi_decl * fd = ffi_decl_new(20);
    
    ffi_decl_delete(fd);
}

int pos_x = 0;
int pos_y = 0;

int turn_left()
{
    printf("turn_left\n");
    return 0;
}

int turn_right()
{
    printf("turn_right\n");
    return 0;
}

int go_ahead(int dist)
{
    printf("go_ahead %d\n", dist);
    
    pos_x += dist;
    pos_y += dist;
    
    return dist;
}

int get_x() { return pos_x; }
int get_y() { return pos_y; }

int fire(int at_x, int at_y)
{
    printf("FIRE! @ (%d, %d)\n", at_x, at_y);
    return 0;
}

int program_one(program * prog)
{
    const char * prog_str =
        "extern \"host\" func turn_left() -> int "
        "extern \"host\" func turn_right() -> int "
        "extern \"host\" func go_ahead(dist : int) -> int "
        "extern \"host\" func get_x() -> int "
        "extern \"host\" func get_y() -> int "
        "extern \"host\" func fire(at_x : int, at_y : int) -> int "
        " "
        "func on_key(dist : int) -> int "
        "{ "
        "   turn_left(); "
        "   go_ahead(dist); "
        "  "
        "   fire(get_x() + 20, get_y() + 30); "
        "  "
        "   0 "
        "}";
    
    return nev_compile_str(prog_str, prog);
}

int program_two(program * prog)
{
    const char * prog_str =
        "extern \"host\" func turn_left() -> int "
        "extern \"host\" func turn_right() -> int "
        "extern \"host\" func go_ahead(dist : int) -> int "
        "extern \"host\" func get_x() -> int "
        "extern \"host\" func get_y() -> int "
        "extern \"host\" func fire(at_x : int, at_y : int) -> int "
        " "
        "func on_key(dist : int) -> int "
        "{ "
        "   turn_right(); "
        "   go_ahead(dist); "
        "  "
        "   fire(get_x() + 20, get_y() + 30); "
        "   fire(get_x() + 25, get_y() + 35); "
        "  "
        "   0 "
        "}";

    return nev_compile_str(prog_str, prog);
}

int execute_prog(program * prog, int param1)
{
    int ret = 0;
    object result = { 0 };

    prog->params[0].int_value = param1;

    vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

    ret =
        nev_execute(prog, "on_key", &result, machine);
    if (ret == 0)
    {
        assert(result.type == OBJECT_INT && result.int_value == 0);
    }

    vm_delete(machine);

    return 0;
}

void test_two()
{
    program * prog_one = program_new();
    program * prog_two = program_new();

    int ret1 = program_one(prog_one);
    int ret2 = program_two(prog_two);
    if (ret1 == 0 && ret2 == 0)
    {
        execute_prog(prog_one, 60);
        execute_prog(prog_two, 40);
    }

    program_delete(prog_one);
    program_delete(prog_two);
}

int main(int argc, char * argv[])
{
    test_zero();
    test_one();
    test_two();
    
    return 0;
}

#endif /* NO_FFI */

