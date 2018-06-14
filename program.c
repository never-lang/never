#include "program.h"
#include <stdlib.h>

program* program_new(object* params, unsigned int param_count,
		     bytecode* code_arr, unsigned int code_size) {
    program* value = malloc(sizeof(program));

    value->params = NULL;
    value->param_count = 0;
    value->code_arr = NULL;
    value->code_size = 0;

    return value;
}

void program_delete(program* value) {
    if (value->params != NULL) {
	object_delete(value->params);
    }
    if (value->code_arr != NULL) {
	bytecode_array_delete(value->code_arr);
    }
    free(value);
}
