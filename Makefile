CC= gcc
CFLAGS= -Werror -Wextra -Wall -std=c99 -pedantic -g -I 'src/includes'

VPATH= src
OBJS_LIB=src/tests/grammar_main.o grammar_check.o linked_list.o stack.o \
grammar_rule.o grammar_command.o grammar_group.o

OBJ=linked_list.o src/lexer.o src/tests/lexer_main.o
all: $(OBJS_LIB)
	$(CC) $(CFLAGS) -o main $(OBJS_LIB)

test: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

.PHONY: test all clean

clean:
	${RM} ${OBJ} ${OBJS_LIB}
