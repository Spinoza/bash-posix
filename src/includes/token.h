#ifndef TOKEN_H
#define TOKEN_H

enum type
{
    IF = 0,
    THEN = 1,
    ELSE = 2,
    FI = 3,
    LOGICAL_AND = 4,
    SEMICOLON = 5,
    WHILE = 6,
    FOR = 7,
    UNTIL = 8,
    CASE = 9,
    DO = 10,
    DONE = 11,
    WORD = 12,
    ASSIGNMENT_W = 13,
    ENDOF = 14,
    PIPE = 15,
    LOGICAL_OR = 16,
    AND = 17,
    IN = 18,
    ESAC = 19,
    ELIF = 20,
    HEREDOC = 21,
    IONUMBER = 22,
    CONDITION = 23,
    ROOT = 24,
    REDIRECTION = 25,
    OPEN_PAR = 26,
    CLOSE_PAR = 27,
    OPEN_BRA = 28,
    CLOSE_BRA = 29,
    TWO_SEMIC = 30
};

struct token
{
    char *name;
    enum type type;
};

#endif /* !TOKEN_H */
