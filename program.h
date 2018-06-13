#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "object.h"
#include "bytecode.h"

typedef struct program
{
    object * params;
    unsigned int param_count;
    bytecode * code_arr;
    unsigned int code_size;
} program;

program * program_new();
void program_delete(program * value);

#endif /* __PROGRAM_H__ */
