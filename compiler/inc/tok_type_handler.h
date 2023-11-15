#ifndef TOK_TYPE_HANDLER_H
#define TOK_TYPE_HANDLER_H

#include <stdlib.h>
#include <string.h>

typedef enum token_t
{
    undefined = 0,

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
    DELIM_BRACE_L,     // (
    DELIM_BRACE_R,     // )
    DELIM_SQ_BRACE_L,  // [
    DELIM_SQ_BRACE_R,  // ]
    DELIM_CUR_BRACE_L, // {
    DELIM_CUR_BRACE_R, // }

    // ENUM SIZE
    _size_
} token_t;

token_t deduce_tok_type(const char **_lxm);

size_t str_len(const char *_str);
size_t str_cmp(const char *_str1, const char *_str2);

#endif