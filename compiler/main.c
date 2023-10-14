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
#include <stdint.h>
#include "file_handler.c"

// ===-------------------------------------------=== GLOBAL VARS
static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]+-/*=,.;:";

// ===-------------------------------------------=== STRUCTS
typedef enum token_t
{
    undefined,
    keyword,
    identifier,
    literal,
    punctuation
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

// ===-------------------------------------------=== helpers
Lexeme *new_lexeme(const char *const *_src, size_t _src_size)
{
    Lexeme *new_lxm = calloc(1, sizeof(Lexeme));
    if (!new_lxm)
    {
        err_ex_p("couldn't allocate memory for a new lexeme", FL);
    }

    new_lxm->txt = (char *)malloc(_src_size);
    if (!new_lxm->txt)
    {
        err_ex_p("couldn't allocate memory for lexeme initialization", FL);
    }

    memcpy(new_lxm->txt, *_src, _src_size);
    *(new_lxm->txt + _src_size) = '\0';
    new_lxm->len = _src_size;

    // todo_err("initialize col and ln", FL);
    new_lxm->col = 0;
    new_lxm->ln = 0;

    return new_lxm;
}

Token *new_token(const char *const *_src, size_t _src_size)
{
    Token *new_tok = calloc(1, sizeof(Token));
    if (!new_tok)
    {
        err_ex_p("couldn't allocate memory for a new token", FL);
    }

    new_tok->lxm = new_lexeme(_src, _src_size);

    return new_tok;
}

void get_string(const char *const *_b, const char **_e)
{
    *_e = *_b + 1;

    int32_t len = strcspn(*_e, "\"") + 1;
    if (len == strlen(*_b))
    {
        err_ex_p("string is missing a closing sign", FL);
    }

    // TODO: substitute special characters

    *_e = *_e + len;
}

void get_char(const char *const *_b, const char **_e)
{
    *_e = *_b + 1;

    int32_t len = strcspn(*_e, "\'") + 1;

    // TODO: substitute special characters

    if (len == strlen(*_b))
    {
        err_ex_p("char is missing a closing sign", FL);
    }
    if (len > 3)
    {
        err_ex_p("char overflow", FL);
    }

    *_e = *_e + len;
}

void push_back_token(Token **_head, Token **_tail, const char *const *_beg, const char *const *_end)
{
    if (!*_head)
    {
        *_head = new_token(_beg, *_end - *_beg);
        *_tail = *_head;
    }
    else
    {
        (*_tail)->next = new_token(_beg, *_end - *_beg);
        *_tail = (*_tail)->next;
    }
}

void skip_white_space(const char **_b, uint32_t *_ln)
{
    while (**_b == ' ' || **_b == '\t' || **_b == '\r' || **_b == '\n')
    {
        if (**_b == '\n')
        {
            (*_ln)++;
        }

        if (**_b == '\0')
        {
            return;
        }

        (*_b)++;
    }
}

void match(const char **_b, const char **_e)
{
    if (**_b == '\"')
    {
        get_string(_b, _e);
        return;
    }
    else if (**_b == '\'')
    {
        get_char(_b, _e);
        return;
    }
    else
    {
        *_e = *_b + strcspn(*_b, delimiters);
    }

    if ((*_e) - (*_b) == 0)
    {
        ++(*_e);
    }
}

// ===-------------------------------------------=== lexer
Token *lex(char **_src)
{   
    const char *beg = *_src, *end = *_src;
    Token *head = NULL, *tail = NULL;
    uint32_t ln = 1;

    while (*end != '\0')
    {
        if (!*_src || !beg || !end)
        {
            err_ex_p("can't lex an empty source", FL);
        }

        skip_white_space(&beg, &ln);
        match(&beg, &end);
        push_back_token(&head, &tail, &beg, &end);
        beg = end;
    }

    if (*_src)
    {
        free(*_src);
        *_src = NULL;
    }

    return head;
}

// ===-------------------------------------------=== parser
void parse(char *_fpath)
{
    char *buff = readf_2buff(_fpath);
    Token *tok_list = lex(&buff); // frees buff

    while (tok_list)
    {
        puts(tok_list->lxm->txt);
        tok_list = tok_list->next;
    }
}

// ===-------------------------------------------=== main
int main(int argc, char **argv)
{
    parse(argv[1]);
    return 0;
}