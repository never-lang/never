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
#include "nev.h"
#include <assert.h>
#include <stdio.h>

void test_one()
{
    int ret = 0;
    object result = { 0 };
    program * prog = program_new();
    const char * prog_str =
        "func main(int a, int b) -> int { return 10 * (a + b); }";

    ret = nev_compile_str(prog_str, prog);
    if (ret == 0)
    {
        prog->params[0].int_value = 2;
        prog->params[1].int_value = 3;

        ret = nev_execute(prog, &result);
        if (ret == 0)
        {
            assert(result.type == OBJECT_INT && result.int_value == 50);
        }

        prog->params[0].int_value = 9;
        prog->params[1].int_value = 1;

        ret = nev_execute(prog, &result);
        if (ret == 0)
        {
            assert(result.type == OBJECT_INT && result.int_value == 100);
        }
    }

    program_delete(prog);
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}
