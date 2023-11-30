#ifndef LEXER_H
#define LEXER_H

#include "file_handler.h"
#include "token_handler.h"

Token *lex(char **_src);
void parse(char *_fpath);

#endif
