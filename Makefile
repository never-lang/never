#
# Copyright 2018 Slawomir Maludzinski
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
CC=gcc
CFLAGS=-Wall -Wextra -Wno-missing-field-initializers -Wno-missing-braces -Wno-unused-function -Wno-unneeded-internal-declaration -Wno-unused-parameter -g
LDLIBS=-lm
LEX=flex
BISON=bison
BFLAGS=--report=solved --defines
AR=ar
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
TESTS=test_object test_scanner test_symtab test_freevar \
      test_vm test_gc test_libmath test_exec

nev: libnev.a main.o

libnev.a: scanner.o parser.o expr.o var.o freevar.o func.o never.o symtab.o \
          typecheck.o gencode.o utils.o bytecode.o vm.o gc.o object.o nev.o \
          constred.o tailrec.o optimize.o libmath.o libvm.o program.o

tests: $(TESTS)

run_tests:
	./test_object
	./test_symtab
	./test_freevar
	./test_vm
	./test_gc
	./test_libmath
	./test_exec

test_object: object.o

test_scanner: scanner.o

test_symtab: symtab.o var.o freevar.o func.o expr.o

test_freevar: symtab.o var.o freevar.o func.o expr.o

test_vm: vm.o gc.o object.o utils.o libmath.o libvm.o expr.o var.o freevar.o func.o symtab.o

test_gc: gc.o object.o

test_libmath: test_libmath.o libmath.o libvm.o func.o expr.o var.o object.o freevar.o gc.o symtab.o utils.o

test_exec: test_exec.o libnev.a

lib%.a: %.o
	$(AR) r $@ $?

%.c : %.y
	$(BISON) $(BFLAGS) -o $@ $<

deps:
	$(CC) -MM *.c *.h > .deps

include .deps

.PHONY: clean
clean:
	@rm -f $(OBJ) $(TESTS) libnev.a nev

