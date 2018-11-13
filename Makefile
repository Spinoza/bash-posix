CC= gcc
CFLAGS= -Werror -Wextra -Wall -std=c99 -pedantic -g -I src/include/

LIB=libsh.a
OBJS_LIB=grammar.o queue.o stack.o

LDFLAGS= -L.
LDLIB= -lsh

all: $(LIB)($(OBJS_LIB))
     $(CC) $(CFLAGS) main.o -o main $(LDFLAGS) $(LDLIB)
