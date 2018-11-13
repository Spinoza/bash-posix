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
    AND = 17
};

struct token
{
    char *name;
    enum type type;
};

#endif /* !TOKEN_H */
