#ifndef TOKEN_H
#define TOKEN_H
/**
  *\file token.h
  *\brief
  *All information about the structures of the tokens
  *and about the enum that gathers all the types a token can be.
  */

/**
  *\enum type
  *\brief A token type enum
  *
  *When part of the command if transformed in a token,
  *a type is given to it. This enum gathers all the types.
  */
enum type
{
    IF = 0, /**< The string parsed is an if. */
    THEN = 1, /**< The string parsed is a then. */
    ELSE = 2, /**< The string parsed is an else. */
    FI = 3, /**< The string parsed is a fi. */
    LOGICAL_AND = 4, /**< The string parsed is a double ampersand. */
    SEMICOLON = 5, /**< The string parsed is one semicolon. */
    WHILE = 6, /**< The string parsed is a while. */
    FOR = 7, /**< The string parsed is a for. */
    UNTIL = 8, /**< The string parsed is an until. */
    CASE = 9, /**< The string parsed is a case. */
    DO = 10, /**< The string parsed is a do. */
    DONE = 11, /**< The string parsed is a done. */
    WORD = 12, /**< The string parsed is a word (might be many things). */
    ASSIGNMENT_W = 13, /**< The string parsed is a word directly followed by an equal and a number. */
    ENDOF = 14, /**< The string parsed is an EOF. */
    PIPE = 15, /**< The string parsed is a pipe. */
    LOGICAL_OR = 16, /**< The string parsed is a double pipe. */
    AND = 17, /**< The string parsed is one ampersand. */
    IN = 18, /**< The string parsed is an in. */
    ESAC = 19, /**< The string parsed is an esace. */
    ELIF = 20, /**< The string parsed is an elif. */
    HEREDOC = 21, /**< The string parsed is a here document. */
    IONUMBER = 22, /**< The string parsed is a number directly followed by a redirection symbol. */
    CONDITION = 23, /**< Used to build the ast tree. */
    ROOT = 24, /**< Used to build the ast tree. */
    REDIRECTION = 25, /**< The string parsed is a redirection symbol. */
    OPEN_PAR = 26, /**< The string parsed is an opening parenthesis. */
    CLOSE_PAR = 27, /**< The string parsed is a closing parenthesis. */
    OPEN_BRA = 28, /**< The string parsed is an opening bracket. */
    CLOSE_BRA = 29, /**< The string parsed is a closing bracket. */
    TWO_SEMIC = 30, /**< The string parsed is a double semicolon. */
    FUNCTION = 31, /**< Used to build the ast tree. */
    EXPAND_W = 32, /**< The string parsed starts with & '$'. */
    OPEN_ARITH, /**< Used in execution for arithmetic expansions **/
    END_ARITH /**< Used in execution for arithmetic expansions **/
};

/**
  *\struct token
  *\brief A token struct
  *
  *This struct gathers information about the tokens created while lexing.
  */
struct token
{
    char *name; /**< The string parsed, name of the token. */
    enum type type; /**< The type associated with the token. */
};

#endif /* !TOKEN_H */
