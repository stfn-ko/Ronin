// ===-------------------------------------------=== INCLUDES
#include "file_handler.c"

// ===-------------------------------------------=== GLOBAL VARS
static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]+-/*=,.;";

// ===-------------------------------------------=== STRUCTS
enum tok_type
{
    tok_keyword,
    tok_identifier,
    tok_literal,
    tok_punctuation,
    tok_delimeter
};

typedef struct Token
{
    char *lexeme;
    enum tok_type type;
    struct Token *next_tok;
} Token;

// size_t min_strcspn(char *_cptr, const char *_cstr1, const char *_cstr2)
// {
//     size_t c1 = strcspn(_cptr, _cstr1);
//     size_t c2 = strcspn(_cptr, _cstr2);
//     return (c1 < c2) ? c1 : c2;
// }



Token *create_token(char *_src, size_t _bytes_to_read)
{
    Token *new_tok = malloc(sizeof(Token));

    if (!new_tok)
        perroex("couldn't allocate memory for a new token");

    memset(new_tok, 0, sizeof(Token));

    return new_tok;
}

// ===-------------------------------------------=== lex
Token *lex(char *_src)
{
    char *beg = _src;
    char *end = _src;

    Token *tok_list, *head = tok_list;

    while (*end != '\0')
    {
        if (!_src || !beg || !end)
            perroex("can't lex an empty source");

        beg += strspn(beg, whitespace);
        end = beg + strcspn(beg, delimiters);     

        // skip one-line comment
        if (*beg == '/' && *(beg + 1) == '/') 
        {
            while (*beg != '\n') ++beg;
            end = ++beg;
            continue;
        }

        if (end - beg == 0)
            end++;

        printf("lexed: %.*s\n", end - beg, beg);
        // tok_list = create_token(beg, end - beg);

        beg = end;
    }

    return head;
}

// ===-------------------------------------------=== parse_expr
void parse_expr(char *_fpath)
{
    char *buff = readf_2buff(_fpath);
    Token *tok_list = lex(buff);
}

// ===-------------------------------------------=== main
int main(int argc, char **argv)
{
    parse_expr(argv[1]);
    return 0;
}