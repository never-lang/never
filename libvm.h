#ifndef __LIBVM_H__
#define __LIBVM_H__

#include "vm.h"
#include "bytecode.h"

void libvm_execute_build_in(vm * machine, bytecode * code);

#endif /* __LIBVM_H__ */


