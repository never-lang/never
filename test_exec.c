#include <stdio.h>
#include <assert.h>
#include "nev.h"

void test_one()
{
    int ret = 0;
    object result = { 0 };
    program * prog = program_new();
    const char * prog_str = "func main(int a, int b) -> int { return 10 * (a + b); }";

    ret = parse_str(prog_str, prog);
    if (ret == 0)
    {
        prog->params[0].int_value = 2;
        prog->params[1].int_value = 3;

        ret = execute(prog, &result);
        if (ret == 0)
        {
            assert(result.type == OBJECT_INT && result.int_value == 50);
        }
        
        prog->params[0].int_value = 9;
        prog->params[1].int_value = 1;

        ret = execute(prog, &result);
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

