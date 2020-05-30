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
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

char * readall(const char * file)
{
    FILE * f = fopen(file, "r");
    fseek(f, 0L, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char * src = (char *)calloc(fsize + 1, sizeof(char));
    fread(src, sizeof(char), fsize, f);
    fclose(f);

    return src;
}

void run(int param1, int param2, program * prog, const char * entry_name)
{
    int ret;
    object result = { 0 };

    prog->params[0].int_value = param1;
    prog->params[1].int_value = param2;

    vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

    ret = nev_execute(prog, entry_name, &result, machine);
    if (ret == 0)
    {
        assert(result.type == OBJECT_INT &&
               result.int_value == 10 * (param1 + param2));
    }

    vm_delete(machine);
}

void test_one()
{
    int ret = 0;
    program * prog = program_new();
    const char * prog_str =
        "func main(a : int, b : int) -> int { 10 * (a + b) }";

    ret = nev_compile_str(prog_str, prog);
    if (ret == 0)
    {
        int param1 = 1;
        int param2 = 1;

        for (param1 = 1; param1 < 10; param1++)
        {
            for (param2 = 1; param2 < 10; param2++)
            {
                run(param1, param2, prog, "main");
            }
        }
    }

    program_delete(prog);
}

void test_two()
{
    int ret = 0;
    program * prog = program_new();
    const char * prog_str = 
        "func on_event(x : int, y : int) -> int { 10 * (x + y) }";
        
    ret = nev_compile_str(prog_str, prog);
    if (ret == 0)
    {
        int param1 = 10;
        int param2 = 20;
        
        run(param1, param2, prog, "on_event");
    }
    
    program_delete(prog);
}

void test_sample(const char * samplepath)
{
    program * prog = program_new();
    char * prog_str = readall(samplepath);

    if (prog_str != NULL)
    {
        object result = { 0 };

        int ret = nev_compile_str(prog_str, prog);
        if (ret != 0)
        {
            printf("path: %s\nprog_str: %s\n", samplepath, prog_str);
        }
        assert(ret == 0);

        vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

        ret = nev_execute(prog, "main", &result, machine);
        if (ret != 0)
        {
            printf("path: %s\nprog_str: %s\n", samplepath, prog_str);
        }

        vm_delete(machine);

        assert(ret == 0);

        free(prog_str);
    }
    
    program_delete(prog);
}

void test_samples(const char * dirpath)
{
    DIR * dir;
    struct dirent * ent;
    if ((dir = opendir(dirpath)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (0 == strcmp(".\0", ent->d_name) || 0 == strcmp("..\0", ent->d_name))
            {
                continue;
            }
            char samplepath[PATH_MAX + 1] = { 0 };
            snprintf(samplepath, PATH_MAX, "%s/%s", dirpath, ent->d_name);
            test_sample((const char *)samplepath);
        }
        closedir(dir);
    }
}

int main(int argc, char * argv[])
{
    char cwd[PATH_MAX] = { 0 };
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }

    test_one();
    test_two();
    test_samples((const char *)"../sample\0");

    return 0;
}
