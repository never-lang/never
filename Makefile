CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-parameter -g
LDLIBS=-lm
LEX=flex
BISON=bison
BFLAGS=--report=solved --defines

nev: scanner.o parser.o expr.o var.o freevar.o func.o never.o symtab.o typecheck.o gencode.o utils.o bytecode.o vm.o gc.o object.o nev.c libmath.o

test_object: object.o

test_scanner: scanner.o

test_symtab: symtab.o var.o freevar.o func.o expr.o

test_freevar: symtab.o var.o freevar.o func.o expr.o

test_vm: vm.o gc.o object.o utils.o

test_gc: gc.o object.o

test_libmath: test_libmath.o libmath.o func.o expr.o var.o freevar.o symtab.o

%.c : %.y
	${BISON} ${BFLAGS} -o $@ $<

deps:
	$(CC) -MM *.c *.h > .deps
 
include .deps


