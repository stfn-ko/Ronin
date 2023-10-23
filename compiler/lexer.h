#ifndef LEXER_H
#define LEXER_H

#include "file_handler.h"

typedef enum token_t
{
    undefined,

    // KEYWORD - PERMISSIONS - READ-ONLY
    KW_READ_ONLY,          // /r
    KW_READ_ONLY_UNIQUE,   // /rx
    KW_READ_ONLY_SHARED,   // /rs
    KW_READ_ONLY_OBSERVER, // /ro

    // KEYWORD - PERMISSIONS - READ-AND-WRITE
    KW_READ_AND_WIRTE,          // /rw
    KW_READ_AND_WIRTE_UNIQUE,   // /rwx
    KW_READ_AND_WIRTE_OBSERVER, // /rwo

    // KEYWORDS - CONTROL STATEMENTS
    KW_RETURN,

    // KEYWORDS - LOOPS
    KW_WHILE,
    KW_FOR,

    // KEYWORDS - LOOPS - CONTROL STATEMENTS
    KW_BREAK,
    KW_SKIP,

    // KEYWORDS - CONTROL FLOW
    KW_IF,
    KW_ELSE,
    KW_MATCH,

    // KEYWORDS - BOOLEAN VALUES
    KW_TRUE,
    KW_FALSE,

    // KEYWORDS - TYPES - SIGNED INTEGERS
    KW_ISIZE,
    KW_I8,
    KW_I16,
    KW_I32,
    KW_I64,
    KW_I128,

    // KEYWORDS - TYPES - UNSIGNED INTEGERS
    KW_USIZE,
    KW_U8,
    KW_U16,
    KW_U32,
    KW_U64,
    KW_U128,

    // KEYWORDS - TYPES - FLOATS
    KW_F32,
    KW_F64,
    KW_F128,

    // KEYWORDS - TYPES - BOOLEAN
    KW_BOOL,

    // KEYWORDS - TYPES - CHAR / STRING (ASCII)
    KW_CHAR,
    KW_STR,
    // KW_CHAR_UTF,
    // KW_STR_UTF,

    // LITERALS - NUMBERS
    LIT_INTEGER,
    LIT_FLOAT,

    // LITERALS - CHARACTERS
    LIT_CHAR_ASCII,
    LIT_CHAR_QUOTE_ESC,
    LIT_CHAR_ASCII_ESC,
    // LIT_CHAR_UNICODE_ESC,

    // LITERALS - STRINGS
    LIT_STRING_ASCII,

    // IDENTIFIERS
    ID,

    // PUNCTUATORS
    PUNC_PLUS,     // +
    PUNC_MINUS,    // -
    PUNC_FW_SLASH, // /
    PUNC_STAR,     // *
    PUNC_PERCENT,  // %
    PUNC_CARET,    // ^
    PUNC_NOT,      // !
    PUNC_AND,      // &
    PUNC_OR,       // |
    PUNC_EQ,       // =
    PUNC_GT,       // >
    PUNC_LT,       // <
    PUNC_AT,       // @
    PUNC_HTAG,     // #
    PUNC_COMMA,    // ,
    PUNC_DOT,      // .
    PUNC_SEMI,     // ;
    PUNC_COL,      // :

    // DELIMITERS
    DELIM_BRACES,     // ()
    DELIM_SQ_BRACES,  // []
    DELIM_CUR_BRACES, // {}

} token_t;

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