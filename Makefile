CC=gcc
CFLAGS=-Wall -g
LDFLAGS=
LEX=flex
BISON=bison
BFLAGS=--report=solved --defines
LFLAGS=--header-file=scanner.h

test_object: object.o

test_scanner: scanner.o

test_parser: scanner.o parser.o

gc: gc.o object.o scanner.o


%.c : %.y
	${BISON} ${BFLAGS} -o $@ $<

