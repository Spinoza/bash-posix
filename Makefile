CC= gcc
CFLAGS= -Werror -Wextra -Wall -std=c99 -pedantic -g -I 'src/includes'

VPATH= src
OBJS_LIB=src/tests/grammar_main.o grammar_check.o linked_list.o stack.o \
grammar_rule.o grammar_command.o grammar_group.o ast.o ast_print.o src/lexer.o

OBJS_MAIN= 42sh.o grammar_check.o linked_list.o stack.o \
grammar_rule.o grammar_command.o grammar_group.o ast.o ast_print.o \
src/lexer.o src/options.o

OBJ=linked_list.o src/lexer.o src/tests/lexer_main.o

all: $(OBJS_LIB)
	$(CC) $(CFLAGS) -o main $(OBJS_LIB)

test: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

42sh: $(OBJS_MAIN)
	$(CC) $(CFLAGS) -o 42sh $(OBJS_MAIN)

.PHONY: test all clean
clean:
	${RM} ${OBJ} ${OBJS_LIB} $(OBJS_MAIN) main 42sh ast.dot
