#include "bytecode.h"
#include <stdio.h>

void test_one()
{
    unsigned int i = 0;

    for (i = 0; i < BYTECODE_END; i++)
    {
        bytecode code = { 0 };

        code.type = i;

        bytecode_op[i].print(&code);
    }
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}
