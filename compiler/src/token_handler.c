#include "../inc/token_handler.h"

/***********************************************************/
/*                    PRIVATE FUNCTIONS                    */
/***********************************************************/

void skip_comments(Lexeme *_lxm, Position *_pos);
void skip_whitespace(Lexeme *_lxm, Position *_pos);

token_t get_char(Lexeme *_lxm);
token_t get_string(Lexeme *_lxm);
token_t get_num_lit(Lexeme *_lxm);
token_t get_permission(Lexeme *_lxm);

/***********************************************************/
/*                  FUNCTION DEFINITIONS                   */
/***********************************************************/

Token *new_token(Token _tok)
{
    Token *new_tok = (Token *)calloc(1, sizeof(Token));
    if (!new_tok)
    {
        err_ex_p("couldn't allocate memory for a new token", FL);
    }

    memcpy(new_tok, &_tok, sizeof(Token));

    return new_tok;
}

const char *new_lexeme(Lexeme *_lxm)
{
    size_t lxm_size = _lxm->end - _lxm->beg;
    char *new_lxm = (char *)malloc(lxm_size);

    if (!new_lxm)
    {
        err_ex_p("couldn't allocate memory for lexeme initialization", FL);
    }

    memcpy(new_lxm, _lxm->beg, lxm_size);
    *(new_lxm + lxm_size) = '\0';

    return new_lxm;
}

Token *tokenize(Lexeme *_lxm, Position *_pos)
{
    skip_whitespace(_lxm, _pos);
    return new_token(match(_lxm, _pos));
}

Token match(Lexeme *_lxm, Position *_pos)
{
    token_t type = undefined;

    // match strings
    if (*_lxm->beg == '\"') 
    {
        type = get_string(_lxm);
    }
    
    // match chars
    else if (*_lxm->beg == '\'') 
    {  
        type = get_char(_lxm);
    }
    
    // match permissions
    else if (*_lxm->beg == '/' && *(_lxm->beg + 1) == 'r')
    {
        type = get_permission(_lxm);
    }
    
    // match numerical literals
    else if (*_lxm->beg >= '0' && *_lxm->beg <= '9')
    {
        type = get_num_lit(_lxm);
    }

    // match anything until delimiter
    else
    {
        _lxm->end = _lxm->beg + strcspn(_lxm->beg, delimiters);
        const char *lxm_buff = new_lexeme(_lxm);
        type = get_type(&lxm_buff); // frees lxm_buff
    }

    if (_lxm->end - _lxm->beg == 0)
    {
        ++_lxm->end;
    }

    Token tok = {new_lexeme(_lxm), type, *_pos, NULL};
    Scope.pos = *_pos;
    return tok;
}

void skip_whitespace(Lexeme *_lxm, Position *_pos)
{
    char beg = *_lxm->beg;

    while (beg == ' ' || beg == '\t' || beg == '\r' || beg == '\n')
    {
        if (beg == '\n')
        {
            _pos->ln++;
            _pos->col = (size_t) (_lxm->end - _lxm->beg + 1);
        }

        if (beg == '\0')
        {
            return;
        }

        beg = *(++_lxm->beg);
    }

    skip_comments(_lxm, _pos);
}

void skip_comments(Lexeme *_lxm, Position *_pos)
{
    if (*_lxm->beg == '/' && *(_lxm->beg + 1) == '/')
    {
        while (*_lxm->beg != '\n')
        {
            ++_lxm->beg;
        }

        skip_whitespace(_lxm, _pos);
    }
}

token_t get_string(Lexeme *_lxm)
{
    _lxm->end = _lxm->beg + 1;

    size_t len = strcspn(_lxm->end, "\"") + 1;
    if (len == strlen(_lxm->beg))
    {
        err_ex_p("string is missing a closing sign", Scope.path, Scope.pos.ln);
    }

    // TODO: substitute special characters

    _lxm->end = _lxm->end + len;
    
    return LIT_STRING_ASCII; 
}

token_t get_char(Lexeme *_lxm)
{
    _lxm->end = _lxm->beg + 1;

    size_t len = strcspn(_lxm->end, "\'") + 1;

    // TODO: substitute special characters

    if (len == strlen(_lxm->beg))
    {
        err_ex_p("char is missing a closing sign", Scope.path, Scope.pos.ln);
    }
    if (len > 3)
    {
        err_ex_p("char overflow", Scope.path, Scope.pos.ln);
    }

    _lxm->end = _lxm->end + len;

    return LIT_CHAR_ASCII; 
}

token_t get_num_lit(Lexeme *_lxm)
{
    char isfloat = 0;
    _lxm->end = _lxm->beg;

    while (*_lxm->end >= '0' && *_lxm->end <= '9')
    {
        if (*(_lxm->end + 1) == '.')
        {
            isfloat = 1;
            _lxm->end += 2;
        }
        else 
        {
            ++_lxm->end;
        }
    }

    if (isfloat)
    {
        return LIT_FLOAT;
    }
    else 
    {
        return LIT_INTEGER;
    }
}

token_t get_permission(Lexeme *_lxm)
{
    _lxm->end = _lxm->beg + 2;
    token_t type = KW_READ_ONLY;

    if (*_lxm->end == 'x')
    {
        ++_lxm->end;
        type = KW_READ_ONLY_UNIQUE;
    }
    else if (*_lxm->end == 's')
    {
        ++_lxm->end;
        type = KW_READ_ONLY_SHARED;
    }
    else if (*_lxm->end == 'w')
    {
        ++_lxm->end;

        if (*_lxm->end == 's')
        {
            ++_lxm->end;
            type = KW_READ_AND_WIRTE_SHARED;
        }
        else if (*_lxm->end == 'x')
        {
            ++_lxm->end;
            type = KW_READ_AND_WIRTE_UNIQUE;
        }

        type = KW_READ_AND_WIRTE;
    }
    
    if (!strchr(whitespace, *_lxm->end))
    {
        _lxm->end = _lxm->beg + 1;
        type = undefined;
    }

    return type;
}
