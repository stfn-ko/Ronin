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

void get_string(const char *const *_beg, const char **_end)
{
    *_end = *_beg + 1;

    int32_t len = strcspn(*_end, "\"") + 1;
    if (len == strlen(*_beg))
    {
        err_ex_p("string is missing a closing sign", FL);
    }

    // TODO: substitute special characters

    *_end = *_end + len;
}

void get_char(const char *const *_beg, const char **_end)
{
    *_end = *_beg + 1;

    int32_t len = strcspn(*_end, "\'") + 1;

    // TODO: substitute special characters

    if (len == strlen(*_beg))
    {
        err_ex_p("char is missing a closing sign", FL);
    }
    if (len > 3)
    {
        err_ex_p("char overflow", FL);
    }

    *_end = *_end + len;
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

void skip_white_space(const char **_beg, uint16_t *_ln, const char **_col)
{
    while (**_beg == ' ' || **_beg == '\t' || **_beg == '\r' || **_beg == '\n')
    {
        if (**_beg == '\n')
        {
            (*_ln)++;
            (*_col) = (*_beg) + 1;
        }

        if (**_beg == '\0')
        {
            return;
        }

        (*_beg)++;
    }
}

void match(const char **_beg, const char **_end)
{
    if (**_beg == '\"')
    {
        get_string(_beg, _end);
        return;
    }
    else if (**_beg == '\'')
    {
        get_char(_beg, _end);
        return;
    }
    else
    {
        *_end = *_beg + strcspn(*_beg, delimiters);
    }

    if ((*_end) - (*_beg) == 0)
    {
        ++(*_end);
    }
}

// ===-------------------------------------------=== lexer
Token *lex(char **_src)
{
    const char *beg = *_src, *end = *_src, *col = *_src;
    Token *head = NULL, *tail = NULL;
    uint16_t ln = 1;

    while (*end != '\0')
    {
        if (!*_src || !beg || !end)
        {
            err_ex_p("can't lex an empty source", FL);
        }

        skip_white_space(&beg, &ln, &col);

        match(&beg, &end);

        push_back_token(&head, &tail, &beg, &end);

        tail->lxm->ln = ln;
        tail->lxm->col = beg - col;

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
        printf("ln: %d | col: %d | len: %d | type: %d\n%s\n\n", tok_list->lxm->ln, tok_list->lxm->col, tok_list->lxm->len, tok_list->type, tok_list->lxm->txt);
        tok_list = tok_list->next;
    }
}

// ===-------------------------------------------=== main
int main(int argc, char **argv)
{
    parse(argv[1]);
    return 0;
}