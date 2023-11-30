#ifndef TOKEN_HANDLER_H
#define TOKEN_HANDLER_H

#include "type_handler.h"

static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]+-/*=,.;:";

typedef struct Position
{
    size_t ln;
    size_t col;
} Position;

struct Scope
{
    const char *path;
    Position pos;
} Scope;


typedef struct Lexeme
{
    const char *beg;
    const char *end;
} Lexeme;

typedef struct Token
{
    const char *lxm;
    token_t type;
    struct Position pos;
    struct Token *next;
} Token;

const char *new_lexeme(Lexeme *_lxm);

Token *new_token(Token _tok);
Token match(Lexeme *_lxm, Position *_pos);
Token *tokenize(Lexeme *_lxm, Position *_pos);

#endif