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

#define MAX_LINE_LEN 1024

char * readall(const char * file)
{
    FILE * f = fopen(file, "r");
    fseek(f, 0L, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    size_t er = 0;
    char * src = (char *)calloc(fsize + 1, sizeof(char));
    er = fread(src, sizeof(char), fsize, f);
    if (er != (size_t)fsize)
    {
        fprintf(stderr, "could not read the while file %s expected %lu read %lu\n",
                        file, fsize, er);
    }
    fclose(f);

    return src;
}

int check_errors(const char * sample_path, program * prog)
{
    char error_file[PATH_MAX] = { 0 };

    snprintf(error_file, PATH_MAX, "%s.err", sample_path);
    FILE * f = fopen(error_file, "r");
    if (f == NULL)
    {
        return 1;
    }

    char * ret = NULL;
    unsigned int msg_idx = 0;
    do
    {
        char line[MAX_LINE_LEN] = { 0 };
        ret = fgets(line, MAX_LINE_LEN, f);
        if (ret != NULL &&
            msg_idx < prog->msg_count)
        {
            line[strcspn(line, "\n")] = 0;
            printf("line: %s", line);
            char * substr = strstr(prog->msg_array[msg_idx], line);
            if (substr == NULL)
            {
                break;
            }
            msg_idx++;
        }
    } while (ret != NULL &&
             msg_idx < prog->msg_count);

    fclose(f);

    if (msg_idx >= prog->msg_count) {
        return 0;
    }

    return 1;
}

void run(program * prog, vm * machine, const char * entry_name, int param1, int param2)
{
    int ret;
    object result = { 0 };

    ret = nev_prepare(prog, entry_name);
    if (ret == 0)
    {
        prog->params[0].int_value = param1;
        prog->params[1].int_value = param2;

        ret = nev_execute(prog, machine, &result);
        if (ret == 0)
        {
            assert(result.type == OBJECT_INT &&
                   result.int_value == 10 * (param1 + param2));
        }
    }
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

        vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

        for (param1 = 1; param1 < 10; param1++)
        {
            for (param2 = 1; param2 < 10; param2++)
            {
                run(prog, machine, "main", param1, param2);
            }
        }

        vm_delete(machine);
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

        vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

        run(prog, machine, "on_event", param1, param2);

        vm_delete(machine);
    }
    
    program_delete(prog);
}

void test_three()
{
    int ret = 0;
    program * prog = program_new();
    const char * prog_str = 
        "func on_event(x : int, y : int) -> int { 10 * (x + y) }";
        
    ret = nev_compile_str(prog_str, prog);
    if (ret == 0)
    {
        object result = { 0 };
        vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

        ret = nev_prepare(prog, "on_event");
        if (ret == 0)
        {
            prog->params[0].int_value = 1;
            prog->params[1].int_value = 2;

            ret = nev_execute(prog, machine, &result);
            if (ret == 0)
            {
                assert(result.type == OBJECT_INT && result.int_value == 30);
            }
            else
            {
                assert(0);
            }

            prog->params[0].int_value = 10;
            prog->params[1].int_value = 20;

            ret = nev_execute(prog, machine, &result);
            if (ret == 0)
            {
                assert(result.type == OBJECT_INT && result.int_value == 300);
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            assert(0);
        }

        vm_delete(machine);
    }
    else
    {
        assert(0);
    }
    
    program_delete(prog);
}

void test_sample(const char * samplepath, unsigned int current, unsigned int count)
{
    program * prog = program_new();
    char * prog_str = readall(samplepath);

    if (prog_str != NULL)
    {
        int ret = nev_compile_str(prog_str, prog);
        if (ret != 0)
        {
            printf("[%d/%d] path: %s\nprog_str: %s\n", current, count, samplepath, prog_str);
            ret = check_errors(samplepath, prog);
        }
        assert(ret == 0);

        ret = nev_prepare(prog, "main");
        if (ret == 0)
        {
            object result = { 0 };
            vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

            ret = nev_execute(prog, machine, &result);
            if (ret != 0)
            {
                printf("path: %s\nprog_str: %s\n", samplepath, prog_str);
            }
            assert(ret == 0);

            vm_delete(machine);
        }

        free(prog_str);
    }
    
    program_delete(prog);
}

int skip_dir(struct dirent * ent)
{
    int skip = 0;

    skip = strcmp(".", ent->d_name) == 0 ||
           strcmp("..", ent->d_name) == 0 ||
           strcmp("lib", ent->d_name) == 0;
    if (skip)
    {
        return skip;
    }

    char * dot = strrchr(ent->d_name, '.');
    if (dot && strcmp(dot, ".err") == 0)
    {
        return 1;
    }

    return skip;
}

void test_samples(const char * dirpath)
{
    DIR * dir;
    struct dirent * ent;
    unsigned int count = 0;
    unsigned int current = 0;

    if ((dir = opendir(dirpath)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (skip_dir(ent))
            {
                continue;
            }
            count++;
        }
        closedir(dir);
    }

    if ((dir = opendir(dirpath)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (skip_dir(ent))
            {
                continue;
            }
            char samplepath[PATH_MAX + 1] = { 0 };
            snprintf(samplepath, PATH_MAX, "%s/%s", dirpath, ent->d_name);
            test_sample((const char *)samplepath, current++, count);
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

    setenv("NEVER_PATH", ".:../sample/lib", 1);

    test_one();
    test_two();
    test_three();
    test_samples((const char *)"../sample");

    return 0;
}
