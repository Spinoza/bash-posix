//! @~English
//! @page 42sh 
//! @section synopsis SYNOPSIS
//!	#include <stdlib.h>\n
//!     #include <string.h>\n
//!     #include "lexer.h"\n
//!     #include "options.h"\n
//!     #include "grammar_check.h"\n
//!     #include "linked_list.h"\n
//!
//!
//!	struct linked_list *lexer(char *input[], int argc);\n
//!	int grammar_check(struct linked_list *tokens);
//!
//! @section description DESCRIPTION
//!
//! @section options OPTIONS
//!
//! 	The 42sh program can be called with several options, all conforming to the standar	  d options of the shell language.\n
//!	The command to call 42sh would be : ./42sh [options] [inputs]\n
//!	The currently supported functions are:\n
//!	--norc		This option deactivates the shell resource reader, telling it not to load any resource file.\n
//!	--ast-print	This option activates the ast-printer. The later shall print the AST resulting of the parsing and lexing of the input string.\n
//!	-c		Followed by an argument string. Sets the special parameter 0 to the first 
//!
//! @section returnval RETURN VALUES
//!
//!     On successful completion, grammar_check() shall return 1. 0 otherwise and in any c        ase of error.\n
//!	On successful completion, lexer() shall return a linked_list containing all tokens        of the input line, correctly parsed. On error, which is basically when a fail of m        alloc(3) occurs, it returns NULL.\n
//!	
//! @section author AUTHORS
//!	Sarah Onfray - sarah.onfray - EPITA 2021\n
//!	Leo Naves - leo.naves - EPITA 2021\n
//!	Salome Desauty - salome.desauty - EPITA 2021\n
//!	Yohann Degli Esposti - yohann.degli-esposti - EPITA 2021\n
//!
