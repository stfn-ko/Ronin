#include "../inc/lexer.h"

/***********************************************************/
/*                    PRIVATE FUNCTIONS                    */
/***********************************************************/

void push_back_token(Token **_head, Token **_tail, Token *_tok);

/***********************************************************/
/*                  FUNCTION DEFINITIONS                   */
/***********************************************************/

Token *lex(char **_src)
{
    Token *head = NULL, *tail = NULL;
    Lexeme lxm = {*_src, *_src};
    Position pos = {1, 0};

    while (*lxm.end != '\0')
    {
        if (!*_src || !lxm.beg || !lxm.end)
        {
            err_ex_p("can't lex an empty source", Scope.path, Scope.pos.ln);
        }

        push_back_token(&head, &tail, tokenize(&lxm, &pos));

        lxm.beg = lxm.end;
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
    Scope.pos.ln = 0;
    Scope.pos.col = 0;
    Scope.path = _fpath; 

    char *buff = readf_2buff(_fpath);
    Token *tok_list = lex(&buff); // frees buf

    while (tok_list)
    {
        printf("ln: %d | col: %d | type: %d\n%s\n\n", 
            tok_list->pos.ln, tok_list->pos.col, tok_list->type, tok_list->lxm);
        tok_list = tok_list->next;
    }
}

void push_back_token(Token **_head, Token **_tail, Token *_tok)
{
    if (!*_head)
    {
        *_head = _tok;
        *_tail = *_head;
    }
    else
    {
        (*_tail)->next = _tok;
        *_tail = (*_tail)->next;
    }

    Scope.pos.ln = _tok->pos.ln;
    Scope.pos.col = _tok->pos.col;
}

