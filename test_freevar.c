#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "freevar.h"

void test_one()
{
    freevar_list * list = freevar_list_new();
    
    freevar_list_delete(list);
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}

