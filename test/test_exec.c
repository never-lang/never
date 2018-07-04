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
#include <stdio.h>
#include <assert.h>
#include "nev.h"

void run(int param1, int param2, program * prog)
{
    int ret;
    object result = { 0 };

    prog->params[0].int_value = param1;
    prog->params[1].int_value = param2;

    ret = nev_execute(prog, &result);
    if (ret == 0)
    {
        assert(result.type == OBJECT_INT && result.int_value == 10 * (param1 + param2));
    }
}

void test_one()
{
    int ret = 0;
    program * prog = program_new();
    const char * prog_str = "func main(a -> int, b -> int) -> int { return 10 * (a + b); }";

    ret = nev_compile_str(prog_str, prog);
    if (ret == 0)
    {
        int param1 = 1;
        int param2 = 1;

        for (param1 = 1; param1 < 10; param1++)
        for (param2 = 1; param2 < 10; param2++)
        {
             run(param1, param2, prog);
        }
    }

    program_delete(prog);
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}

