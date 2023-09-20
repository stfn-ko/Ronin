/*
    Table of contents:
    - INCLUDES
    - GLOBAL VARS
    - STRUCTS
    - helpers
    - lexer
    - parser
    - main
*/

// ===-------------------------------------------=== INCLUDES
#include <ctype.h>
#include "file_handler.c"

// ===-------------------------------------------=== GLOBAL VARS
static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]+-/*=,.;:\"\'";

// ===-------------------------------------------=== STRUCTS
typedef enum token_t
{
    undefined,
    keyword,
    identifier,
    literal,
    punctuation
} token_t;

typedef struct Lexme
{
    char *val;
    size_t ln;
    size_t row;
    size_t len;
} Lexme;

typedef struct Token
{
    Lexme *lxm;
    token_t type;
    struct Token *next;
} Token;

// size_t min_strcspn(char *_cptr, const char *_cstr1, const char *_cstr2)
// {
//     size_t c1 = strcspn(_cptr, _cstr1);
//     size_t c2 = strcspn(_cptr, _cstr2);
//     return (c1 < c2) ? c1 : c2;
// }

// ===-------------------------------------------=== helpers
Lexme *new_lexeme(const char *_src, size_t _src_size)
{
    Lexme *new_lxm = calloc(1, sizeof(Lexme));
    if (!new_lxm)
    {
        err_ex_p("couldn't allocate memory for a new lexeme", FL);
    }

    new_lxm->val = (char *)malloc(_src_size);
    if (!new_lxm->val)
    {
        err_ex_p("couldn't allocate memory for lexeme initialization", FL);
    }

    memcpy(new_lxm->val, _src, _src_size);
    *(new_lxm->val + _src_size) = '\0';
    new_lxm->len = _src_size;
    
    new_lxm->row = 0;
    new_lxm->ln = 0;

    return new_lxm;
}

Token *new_token(char *_src, size_t _src_size)
{
    Token *new_tok = calloc(1, sizeof(Token));
    if (!new_tok)
    {
        err_ex_p("couldn't allocate memory for a new token", FL);
    }

    new_tok->lxm = new_lexeme((const char *)_src, _src_size);

    return new_tok;
}

// ===-------------------------------------------=== lexer
Token *lex(char *_src)
{
    char *beg = _src;
    char *end = _src;

    Token *head_node = NULL, *current_node = NULL;

    while (*end != '\0')
    {
        if (!_src || !beg || !end)
            err_ex_p("can't lex an empty source", FL);

        beg += strspn(beg, whitespace);
        end = beg + strcspn(beg, delimiters);

        if (end - beg == 0)
        {
            end++;
        }

        if (!head_node)
        {
            head_node = new_token(beg, end - beg);
            current_node = head_node;
        }
        else
        {
            current_node->next = new_token(beg, end - beg);
            current_node = current_node->next;
        }

        beg = end;
    }

    return head_node;
}

// ===-------------------------------------------=== parser
void parse(char *_fpath)
{
    char *buff = readf_2buff(_fpath);
    Token *tok_list = lex(buff);
    while (tok_list)
    {
        puts(tok_list->lxm->val);
        tok_list = tok_list->next;
    }
}

// ===-------------------------------------------=== main
int main(int argc, char **argv)
{
    parse(argv[1]);

    return 0;
}