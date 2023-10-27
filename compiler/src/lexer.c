#include "../inc/lexer.h"

static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]+-/*=,.;:";

struct Scope
{
    const char *path;
    size_t ln;
    size_t col;
} Scope = {NULL, 0, 0};

/***********************************************************/
/*                    PRIVATE FUNCTIONS                    */
/***********************************************************/

void push_back_token(Token **_head, Token **_tail, const char *const *_beg, const char *const *_end);
void skip_white_space(const char **_beg, size_t *_ln, const char **_col);
void skip_comments(const char **_beg, size_t *_ln, const char **_col);
void get_string(const char *const *_beg, const char **_end);
void get_char(const char *const *_beg, const char **_end);
void match(const char **_beg, const char **_end);
void deduce_token_type(Token **_t);

/***********************************************************/
/*                    PUBLIC FUNCTIONS                     */
/***********************************************************/

Lexeme *new_lexeme(const char *const *_src, size_t _src_size)
{
    Lexeme *new_lxm = (Lexeme *)calloc(1, sizeof(Lexeme));
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
    Token *new_tok = (Token *)calloc(1, sizeof(Token));
    if (!new_tok)
    {
        err_ex_p("couldn't allocate memory for a new token", FL);
    }

    new_tok->lxm = new_lexeme(_src, _src_size);

    return new_tok;
}

Token *lex(char **_src)
{
    const char *beg = *_src, *end = *_src, *col = *_src;
    Token *head = NULL, *tail = NULL;
    size_t ln = 1;

    while (*end != '\0')
    {
        if (!*_src || !beg || !end)
        {
            err_ex_p("can't lex an empty source", Scope.path, Scope.ln);
        }

        skip_white_space(&beg, &ln, &col);

        match(&beg, &end);

        push_back_token(&head, &tail, &beg, &end);

        tail->type = deduce_tok_type((const char **)&tail->lxm->txt);

        tail->lxm->ln = ln;
        tail->lxm->col = beg - col;

        Scope.ln = ln;
        Scope.col = beg - col;

        beg = end;
    }

    if (*_src)
    {
        free(*_src);
        *_src = NULL;
    }

    return head;
}

void parse(char *_fpath)
{
    Scope.path = _fpath;

    char *buff = readf_2buff(_fpath);
    Token *tok_list = lex(&buff); // frees buff

    while (tok_list)
    {
        printf("ln: %d | col: %d | len: %d | type: %d\n%s\n\n", tok_list->lxm->ln, tok_list->lxm->col, tok_list->lxm->len, tok_list->type, tok_list->lxm->txt);
        tok_list = tok_list->next;
    }
}

/***********************************************************/
/*                  FUNCTION DECLARATIONS                  */
/***********************************************************/

void get_string(const char *const *_beg, const char **_end)
{
    *_end = *_beg + 1;

    size_t len = strcspn(*_end, "\"") + 1;
    if (len == strlen(*_beg))
    {
        err_ex_p("string is missing a closing sign", Scope.path, Scope.ln);
    }

    // TODO: substitute special characters

    *_end = *_end + len;
}

void get_char(const char *const *_beg, const char **_end)
{
    *_end = *_beg + 1;

    size_t len = strcspn(*_end, "\'") + 1;

    // TODO: substitute special characters

    if (len == strlen(*_beg))
    {
        err_ex_p("char is missing a closing sign", Scope.path, Scope.ln);
    }
    if (len > 3)
    {
        err_ex_p("char overflow", Scope.path, Scope.ln);
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

void skip_comments(const char **_beg, size_t *_ln, const char **_col)
{
    if (**_beg == '/' && *(*_beg + 1) == '/')
    {
        while (**_beg != '\n')
        {
            (*_beg)++;
        }

        skip_white_space(_beg, _ln, _col);
    }
}

void skip_white_space(const char **_beg, size_t *_ln, const char **_col)
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

    skip_comments(_beg, _ln, _col);
}

void match(const char **_beg, const char **_end)
{
    if (**_beg == '\"') // match strings
    {
        get_string(_beg, _end);
        return;
    }
    else if (**_beg == '\'') // match chars
    {
        get_char(_beg, _end);
        return;
    }
    else // match anything until delimiter
    {
        *_end = *_beg + strcspn(*_beg, delimiters);
    }

    if ((*_end) - (*_beg) == 0)
    {
        ++(*_end);
    }
}
