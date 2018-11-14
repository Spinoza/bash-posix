CC= gcc
CFLAGS= -Werror -Wextra -Wall -std=c99 -pedantic -g -I 'src/includes'

VPATH= src
OBJS_LIB=src/tests/grammar_main.o grammar_check.o linked_list.o stack.o grammar_rule.o grammar_command.o grammar_group.o

all: $(OBJS_LIB)
	$(CC) $(CFLAGS) -o main $(OBJS_LIB)
