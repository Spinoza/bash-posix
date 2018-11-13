#ifndef TOKEN_H
#define TOKEN_H

enum type
{
    IF,
    THEN,
    ELSE,
    FI,
    LOGICAL_AND,
    SEMICOLON,
    WHILE,
    FOR,
    UNTIL,
    CASE,
    DO,
    DONE,
    WORD,
    ASSIGNMENT_W,
    ENDOF
};

struct token
{
    char *name;
    enum type type;
};

#endif /* !TOKEN_H */
