CC= gcc
CFLAGS= -Werror -Wextra -Wall -std=c99 -pedantic -g -I 'src/includes'

VPATH= src
OBJS_LIB=main.o grammar.o linked_list.o stack.o


all: $(OBJS_LIB)
	$(CC) $(CFLAGS) -o main $(OBJS_LIB)
