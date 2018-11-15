//! @~English
//! @page 42sh 
//! @section synopsis SYNOPSIS
//!    #include <stdlib.h>\n
//!    #include <string.h>\n
//!    #include "lexer.h"\n
//!    #include "options.h"\n
//!    #include "grammar_check.h"\n
//!    #include "linked_list.h"\n
//!    #include "ast.h"\n
//!    #include "execution_ast.h"
//!
//!	struct linked_list *lexer(char *input[], int argc);\n
//!	int grammar_check(struct linked_list *tokens);\n
//!     struct node *build_ast(struct linked_list *tokens);\n
//!     void print_ast(struct node *ast);\n
//!     [FIXME]
//!
//!	The program 42sh remains a project and, as thus, needs to be compiled before 
//!     being used.\n
//!	The compilation process is as follows : mkdir build at root, cd build, cmake
//!     .. make\n
//!	The program is then launched as follows : ./42sh [-/+option] [input line]\n
//! @section description DESCRIPTION
//!	The 42sh project is a command language interpreter base on the sh(1p) interp
//!     erter.Thus it follows its behaviour: the standard input, a command line script
//!     or a file, is read and executed. But before execution, it check that the
//!     commands are expressed in the conform LL grammar. It is composed with [FIXME]4 func
//!     tions:\n
//!     The lexer() function splits an input line from a file, the terminal, or a st
//!     ring, into tokens.\n
//!     The grammar_check() function checks if a list of token is conform to the LL 
//!     grammar.\n
//!     To execute the command line, all the token are organized in an AST tree. To 
//!     create the tree the function build_ast() is called. Then the the print_ast()
//!     function can be called to print the AST created.
//! @section options OPTIONS
//!
//! 	The 42sh program can be called with several options, all conforming to the s
//!     tandard options of the shell language.\n
//!	The command to call 42sh would be : ./42sh [options] [inputs]\n
//!	The currently supported functions are:\n
//!	--norc		This option deactivates the shell resource reader, telling i
//!     t not to load any resource file.\n
//!	--ast-print	This option activates the ast-printer. The later shall print
//!      the AST resulting of the parsing and lexing of the input string.\n
//!	-c		Followed by an argument string. Sets the special parameter 0
//!      to the first parameter after command line. Then, sets all positionnal param
//!     eters and executes the command given in argument. No command shall be read f
//!     rom standard input.\n
//!	-/+o		Sets or unsets the option given as parameter. Calling -o [op
//!     tion] shall set the option for the bash and calling +o [option] shall unset it.
//!	--version	Prints the actual version of the 42sh program. 
//!
//! @section returnval RETURN VALUES
//!
//!     On successful completion, grammar_check() shall return 1. 0 otherwise and in
//!     any case of error.\n
//!	On successful completion, lexer() shall return a linked_list containing all t
//!     okens of the input line, correctly parsed. On error, which is basically when
//!      a fail of malloc(3) occurs, it returns NULL.\n
//!	
//! @section author AUTHORS
//!	Sarah Onfray - sarah.onfray - EPITA 2021\n
//!	Leo Naves - leo.naves - EPITA 2021\n
//!	Salome Desauty - salome.desauty - EPITA 2021\n
//!	Yohann Degli Esposti - yohann.degli-esposti - EPITA 2021\n
//!
