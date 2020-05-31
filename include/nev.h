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
#ifndef __NEV_H__
#define __NEV_H__

#define DEFAULT_VM_MEM_SIZE 5000
#define DEFAULT_VM_STACK_SIZE 200

#include "object.h"
#include "program.h"
#include "vm.h"

int nev_compile_str(const char * str, program * prog);
int nev_compile_file(const char * file_name, program * prog);

int nev_prepare(program * prog, const char * entry_name);
int nev_prepare_argv_argv(program * prog, const char * entry_name, unsigned int argc, char * argv[]);

int nev_execute(program * prog, vm * machine, object * result);

int nev_compile_file_and_exec(const char * file_name, unsigned int argc,
                              char * argv[], object * result,
                              unsigned int mem_size, unsigned int stack_size);
int nev_compile_str_and_exec(const char * src, unsigned int argc, char * argv[],
                             object * result, unsigned int mem_size,
                             unsigned int stack_size);

#endif /* __NEV_H__ */

