#include <stdio.h>
#include "math.h"
#include "libmath.h"

void test_one()
{
    func * fsin = libmath_func_sin_new();
    
    func_delete(fsin);
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}

