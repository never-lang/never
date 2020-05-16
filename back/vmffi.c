/**
 * Copyright 2020 Slawomir Maludzinski
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
#include "vmffi.h"
#include "gc.h"
#include "module.h"
#include "fficall.h"
#include "dlcache.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef NO_FFI

static unsigned int vm_execute_func_ffi_align(unsigned int value, unsigned int alignment)
{
    if (alignment == 1) return value;
    return (value + (alignment - 1)) & ~(alignment - 1);
}

static ffi_type * vm_execute_func_ffi_record_type(vm * machine, unsigned int count)
{
    ffi_type * type = (ffi_type *)malloc(sizeof(ffi_type));

    type->size = 0;
    type->alignment = 0;
    type->type = FFI_TYPE_STRUCT;
    type->elements = (ffi_type **)malloc((count + 1) * sizeof(ffi_type *));
    type->elements[count] = NULL;

    unsigned int i = 0;
    for (i = 0; i < count; i++)
    {
        bytecode bc = machine->prog->module_value->code_arr[machine->ip++];
        switch (bc.type)
        {
            case BYTECODE_FUNC_FFI_INT:
            {
                type->elements[i] = &ffi_type_sint;
            }
            break;
            case BYTECODE_FUNC_FFI_FLOAT:
            {
                type->elements[i] = &ffi_type_float;
            }
            break;
            case BYTECODE_FUNC_FFI_CHAR:
            {
                type->elements[i] = &ffi_type_schar;
            }
            break;
            case BYTECODE_FUNC_FFI_STRING:
            {
                type->elements[i] = &ffi_type_pointer;
            }
            break;
            case BYTECODE_FUNC_FFI_C_PTR:
            {
                type->elements[i] = &ffi_type_pointer;
            }
            break;
            case BYTECODE_FUNC_FFI_RECORD:
            {
                ffi_type * rec_value = vm_execute_func_ffi_record_type(machine, bc.ffi_record.count);
                type->elements[i] = rec_value;
            }
            break;
            case BYTECODE_FUNC_FFI_VOID:
                assert(0);
            break;
            default:
                assert(0);
        }
    }

    return type;
}

static int vm_execute_func_ffi_record_value(vm * machine, mem_ptr rec_addr, unsigned int count,
                                            ffi_type * type, void * data, unsigned int * offset)
{
    unsigned int i = 0;
    unsigned int ret = 0;

    for (i = 0; i < count; i++)
    {
        bytecode bc = machine->prog->module_value->code_arr[machine->ip++];
        switch (bc.type)
        {
            case BYTECODE_FUNC_FFI_INT:
            {
                mem_ptr int_addr = gc_get_vec(machine->collector, rec_addr, i);
                int * int_value = gc_get_int_ptr(machine->collector, int_addr);

                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);
                *(int *)((char *)data + *offset) = *int_value;
                *offset += type->elements[i]->size;
            }
            break;
            case BYTECODE_FUNC_FFI_FLOAT:
            {
                mem_ptr float_addr = gc_get_vec(machine->collector, rec_addr, i);
                float * float_value = gc_get_float_ptr(machine->collector, float_addr);

                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);
                *(float *)((char *)data + *offset) = *float_value;
                *offset += type->elements[i]->size;
            }
            break;
            case BYTECODE_FUNC_FFI_CHAR:
            {
                mem_ptr char_addr = gc_get_vec(machine->collector, rec_addr, i);
                char * char_value = gc_get_char_ptr(machine->collector, char_addr);

                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);
                *(char *)((char *)data + *offset) = *char_value;
                *offset += type->elements[i]->size;
            }
            break;
            case BYTECODE_FUNC_FFI_STRING:
            {
                mem_ptr str_addr = gc_get_vec(machine->collector, rec_addr, i);
                mem_ptr str_b = gc_get_string_ref(machine->collector, str_addr);

                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);
                if (str_b != nil_ptr)
                {
                    char ** str_value = gc_get_string_ptr(machine->collector, str_b);
                    *(char **)((char **)data + *offset) = *str_value;
                }
                else
                {
                    ret = 1;
                }
                *offset += type->elements[i]->size;
            }
            break;
            case BYTECODE_FUNC_FFI_C_PTR:
            {
                mem_ptr c_ptr_addr = gc_get_vec(machine->collector, rec_addr, i);
                void * c_ptr_value = gc_get_c_ptr(machine->collector, c_ptr_addr);

                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);
                *(void **)((void **)data + *offset) = c_ptr_value;
                *offset += type->elements[i]->size;
            }
            break;
            case BYTECODE_FUNC_FFI_RECORD:
            {
                unsigned int rec_offset = 0;
                mem_ptr rec_ref_addr = gc_get_vec(machine->collector, rec_addr, i);
                mem_ptr rec_addr = gc_get_vec_ref(machine->collector, rec_ref_addr);

                *offset = rec_offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);
                if (rec_addr != nil_ptr)
                {
                    ret |= vm_execute_func_ffi_record_value(machine, rec_addr, bc.ffi_record.count, type->elements[i], data, offset);
                }
                else
                {
                    ret = 1;
                    machine->ip += bc.ffi_record.total_count - 1;
                }
                *offset = rec_offset + type->elements[i]->size;
            }
            break;
            case BYTECODE_FUNC_FFI_VOID:
                assert(0);
            break;
            default:
                assert(0);
        }
    }

    return ret;
}

static mem_ptr vm_execute_func_ffi_record_new(vm * machine, unsigned int count,
                                              ffi_type * type, void * data, unsigned int * offset)
{
    unsigned int rec_offset = 0;

    *offset = rec_offset = vm_execute_func_ffi_align(*offset, type->alignment);

    mem_ptr rec = gc_alloc_vec(machine->collector, count);

    unsigned int i = 0;
    for (i = 0; i < count; i++)
    {
        bytecode bc = machine->prog->module_value->code_arr[machine->ip++];
        switch (bc.type)
        {
            case BYTECODE_FUNC_FFI_INT:
            {
                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);

                int int_value = *(int *)((char *)data + *offset);
                mem_ptr int_addr = gc_alloc_int(machine->collector, int_value);

                *offset += type->elements[i]->size;

                gc_set_vec(machine->collector, rec, i, int_addr);
            }
            break;
            case BYTECODE_FUNC_FFI_FLOAT:
            {
                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);

                float float_value = *(float *)((char *)data + *offset);
                mem_ptr float_addr = gc_alloc_float(machine->collector, float_value);

                *offset += type->elements[i]->size;

                gc_set_vec(machine->collector, rec, i, float_addr);
            }
            break;
            case BYTECODE_FUNC_FFI_CHAR:
            {
                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);

                char char_value = *(char *)((char *)data + *offset);
                mem_ptr char_addr = gc_alloc_char(machine->collector, char_value);

                *offset += type->elements[i]->size;

                gc_set_vec(machine->collector, rec, i, char_addr);
            }
            break;
            case BYTECODE_FUNC_FFI_STRING:
            {
                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);

                char * str_value = *(char **)((char *)data + *offset);
                mem_ptr str_addr = gc_alloc_string(machine->collector, str_value);
                mem_ptr str_ref_addr = gc_alloc_string_ref(machine->collector, str_addr);

                *offset += type->elements[i]->size;

                gc_set_vec(machine->collector, rec, i, str_ref_addr);
            }
            break;
            case BYTECODE_FUNC_FFI_C_PTR:
            {
                *offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);
                
                void * c_ptr_value  = *(void **)((void *)data + *offset);
                mem_ptr c_ptr_addr = gc_alloc_c_ptr(machine->collector, c_ptr_value);

                *offset += type->elements[i]->size;

                gc_set_vec(machine->collector, rec, i, c_ptr_addr);
            }
            break;
            case BYTECODE_FUNC_FFI_RECORD:
            {
                unsigned int rec_offset = 0;
                *offset = rec_offset = vm_execute_func_ffi_align(*offset, type->elements[i]->alignment);

                mem_ptr rec_addr = vm_execute_func_ffi_record_new(machine, bc.ffi_record.count,
                                                                  type->elements[i], data , offset);

                *offset = rec_offset + type->elements[i]->size;

                gc_set_vec(machine->collector, rec, i, rec_addr);            
            }
            break;
            case BYTECODE_FUNC_FFI_VOID:
                assert(0);
            break;
            default:
                assert(0);
        }
    }

    *offset = rec_offset + type->size;

    rec = gc_alloc_vec_ref(machine->collector, rec);

    return rec;
}
#endif /* NO_FFI */

void vm_execute_func_ffi(vm * machine, bytecode * code)
{
#ifndef NO_FFI
    bytecode bc = { 0 };
    int ret = FFI_SUCC;
    unsigned int i = 0;
    unsigned int ffi_count = code->ffi.count;
        
    ffi_decl * fd = ffi_decl_new(code->ffi.count);

    /* prepare param types */
    unsigned int ip = machine->ip;
    for (i = 0; i < ffi_count; i++)
    {
        bc = machine->prog->module_value->code_arr[machine->ip++];
        switch (bc.type)
        {
            case BYTECODE_FUNC_FFI_INT:
                ffi_decl_set_param_type(fd, i, &ffi_type_sint);
            break;
            case BYTECODE_FUNC_FFI_FLOAT:
                ffi_decl_set_param_type(fd, i, &ffi_type_float);
            break;
            case BYTECODE_FUNC_FFI_CHAR:
                ffi_decl_set_param_type(fd, i, &ffi_type_schar);
            break;
            case BYTECODE_FUNC_FFI_STRING:
                ffi_decl_set_param_type(fd, i, &ffi_type_pointer);
            break;
            case BYTECODE_FUNC_FFI_C_PTR:
                ffi_decl_set_param_type(fd, i, &ffi_type_pointer);
            break;
            case BYTECODE_FUNC_FFI_RECORD:
            {
                ffi_type * rec_type = vm_execute_func_ffi_record_type(machine, bc.ffi_record.count);
                ffi_decl_set_param_type(fd, i, rec_type);
            }
            break;
            case BYTECODE_FUNC_FFI_VOID:
                assert(0);
            break;
            default:
                assert(0);
        }
    }

    /* set return */
    bc = machine->prog->module_value->code_arr[machine->ip++];
    switch (bc.type)
    {
        case BYTECODE_FUNC_FFI_INT:
            ffi_decl_set_ret_type(fd, &ffi_type_sint);
        break;
        case BYTECODE_FUNC_FFI_FLOAT:
            ffi_decl_set_ret_type(fd, &ffi_type_float);
        break;
        case BYTECODE_FUNC_FFI_CHAR:
            ffi_decl_set_ret_type(fd, &ffi_type_schar);
        break;
        case BYTECODE_FUNC_FFI_STRING:
            ffi_decl_set_ret_type(fd, &ffi_type_pointer);
        break;
        case BYTECODE_FUNC_FFI_VOID:
            ffi_decl_set_ret_type(fd, &ffi_type_void);
        break;
        case BYTECODE_FUNC_FFI_C_PTR:
            ffi_decl_set_ret_type(fd, &ffi_type_pointer);
        break;
        case BYTECODE_FUNC_FFI_RECORD:
        {
            ffi_type * rec_type = vm_execute_func_ffi_record_type(machine, bc.ffi_record.count);
            ffi_decl_set_ret_type(fd, rec_type);
        }
        break;
        default:
            assert(0);
    }

    /* prepare call */
    ret = ffi_decl_prepare(fd);
    if (ret != FFI_SUCC)
    {
        ffi_decl_delete(fd);

        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_FFI_FAIL;
        return;
    }
    
    /* prepare values */
    machine->ip = ip;
    unsigned int prep_vals = 0;
    for (i = 0; i < code->ffi.count; i++)
    {
        bc = machine->prog->module_value->code_arr[machine->ip++];
        switch (bc.type)
        {
            case BYTECODE_FUNC_FFI_INT:
            {
                int * int_value = gc_get_int_ptr(machine->collector, machine->stack[machine->sp--].addr);
                ffi_decl_set_param_value(fd, i, int_value);
            }
            break;
            case BYTECODE_FUNC_FFI_FLOAT:
            {
                float * float_value = gc_get_float_ptr(machine->collector, machine->stack[machine->sp--].addr);
                ffi_decl_set_param_value(fd, i, float_value);
            }
            break;
            case BYTECODE_FUNC_FFI_CHAR:
            {
                char * char_value = gc_get_char_ptr(machine->collector, machine->stack[machine->sp--].addr);
                ffi_decl_set_param_value(fd, i, char_value);
            }
            break;
            case BYTECODE_FUNC_FFI_STRING:
            {
                mem_ptr str_b = gc_get_string_ref(machine->collector, machine->stack[machine->sp--].addr);
                if (str_b != nil_ptr)
                {
                    char ** str_value = gc_get_string_ptr(machine->collector, str_b);
                    ffi_decl_set_param_value(fd, i, str_value);
                }
                else
                {
                    prep_vals = 1;
                }
            }
            break;
            case BYTECODE_FUNC_FFI_C_PTR:
            {
                void * c_ptr_value = gc_get_c_ptr_ptr(machine->collector, machine->stack[machine->sp--].addr);
                ffi_decl_set_param_value(fd, i, c_ptr_value);
            }
            break;
            case BYTECODE_FUNC_FFI_RECORD:
            {
                unsigned int offset = 0;
                void * rec_value = (void *)malloc(fd->param_types[i]->size);
                memset(rec_value, 0, fd->param_types[i]->size);

                mem_ptr rec_addr = gc_get_vec_ref(machine->collector, machine->stack[machine->sp--].addr);
                if (rec_addr != nil_ptr)
                {
                    prep_vals = vm_execute_func_ffi_record_value(machine, rec_addr, bc.ffi_record.count, fd->param_types[i], rec_value, &offset);
                }
                else
                {
                    prep_vals = 1;
                    machine->ip += bc.ffi_record.total_count - 1;
                }

                ffi_decl_set_param_value(fd, i, rec_value);
            }
            break;
            case BYTECODE_FUNC_FFI_VOID:
                assert(0);
            break;
            default:
                assert(0);
        }
    }

    if (prep_vals == 1)
    {
        ffi_decl_delete(fd);

        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_FFI_FAIL;
        return;
    }

    /* call */
    gc_stack entry = { 0 };
    mem_ptr addr = { 0 };
    char ** strtab_array = NULL;
    void * handle = NULL;

    assert(machine->prog->module_value != NULL);
    strtab_array = machine->prog->module_value->strtab_array;

    handle = dlcache_get_handle(machine->dlib_cache, strtab_array[code->ffi.libname_index]);
    if (handle == NULL)
    {
        ffi_decl_delete(fd);

        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_FFI_FAIL;
        return;
    }
    
    ret = ffi_decl_call(fd, strtab_array[code->ffi.fname_index], handle);
    if (ret != FFI_SUCC)
    {
        ffi_decl_delete(fd);

        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_FFI_FAIL;
        return;
    }

    /* get result */
    bc = machine->prog->module_value->code_arr[machine->ip++];
    switch (bc.type)
    {
        case BYTECODE_FUNC_FFI_INT:
            addr = gc_alloc_int(machine->collector, fd->ret_int_value);
        break;
        case BYTECODE_FUNC_FFI_FLOAT:
            addr = gc_alloc_float(machine->collector, fd->ret_float_value);
        break;
        case BYTECODE_FUNC_FFI_CHAR:
            addr = gc_alloc_char(machine->collector, fd->ret_char_value);
        break;
        case BYTECODE_FUNC_FFI_STRING:
        {
            mem_ptr str = gc_alloc_string(machine->collector, fd->ret_string_value);
            addr = gc_alloc_string_ref(machine->collector, str);
        }
        break;
        case BYTECODE_FUNC_FFI_VOID:
            addr = gc_alloc_int(machine->collector, 0);
        break;
        case BYTECODE_FUNC_FFI_C_PTR:
            addr = gc_alloc_c_ptr(machine->collector, fd->ret_void_value);
        break;
        case BYTECODE_FUNC_FFI_RECORD:
        {
            unsigned int offset = 0;
            addr = vm_execute_func_ffi_record_new(machine, bc.ffi_record.count,
                                                  fd->ret_type, fd->ret_void_value, &offset);
        }
        break;
        default:
            assert(0);
    }

    ffi_decl_delete(fd);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
#endif /* NO_FFI */
}

void vm_execute_func_ffi_int(vm * machine, bytecode * code)
{
    /* func_ffi reads it */
}

void vm_execute_func_ffi_float(vm * machine, bytecode * code)
{
    /* func_ffi reads it */
}

void vm_execute_func_ffi_char(vm * machine, bytecode * code)
{
    /* func_ffi reads it */
}

void vm_execute_func_ffi_string(vm * machine, bytecode * code)
{
    /* func_ffi reads it */
}

void vm_execute_func_ffi_void(vm * machine, bytecode * code)
{
    /* func_ffi reads it */
}

void vm_execute_func_ffi_c_ptr(vm * machine, bytecode * code)
{
    /* func_ffi reads it */
}

void vm_execute_func_ffi_record(vm * machine, bytecode * code)
{
    /* func_ffi reads it */
}

