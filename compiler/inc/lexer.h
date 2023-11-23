#ifndef LEXER_H
#define LEXER_H

#include "file_handler.h"
#include "token_handler.h"

static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]+-/*=,.;:";

struct Scope
{
    const char *path;
    size_t ln;
    size_t col;
} Scope;

typedef struct Lexeme
{
    char *txt;
    size_t ln;
    size_t col;
    size_t len;
} Lexeme;

typedef struct Token
{
    Lexeme *lxm;
    token_t type;
    struct Token *next;
} Token;

Lexeme *new_lexeme(const char *const *_src, size_t _src_size);
Token *new_token(const char *const *_src, size_t _src_size);
Token *lex(char **_src);
void parse(char *_fpath);


#endif