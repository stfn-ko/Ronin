// ===-------------------------------------------=== INCLUDES
#include "file_handler.c"

// ===-------------------------------------------=== GLOBAL VARS
static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]+-/*=,.;";

// ===-------------------------------------------=== STRUCTS
typedef enum TokenType
{
    tok_undefined,
    tok_keyword,
    tok_identifier,
    tok_literal,
    tok_punctuation,
    tok_delimeter
} TokenType;

typedef struct Token
{
    char *lexeme;
    TokenType type;
    struct Token *next_tok;
} Token;

// size_t min_strcspn(char *_cptr, const char *_cstr1, const char *_cstr2)
// {
//     size_t c1 = strcspn(_cptr, _cstr1);
//     size_t c2 = strcspn(_cptr, _cstr2);
//     return (c1 < c2) ? c1 : c2;
// }

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

        if (end - beg == 0)
            end++;

        printf("lexed: %.*s\n", end - beg, beg);

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

void get_flags(const int _flagc, const char **flagv)
{

    for (int i = 0; i < _flagc; ++i)
    {        
        if (!strcmp(flagv[i], "--lean-ron")) print_lean_ronin();
        else if (!strcmp(flagv[i], "--uni-ron")) print_universal_ronin();
    }
}

// ===-------------------------------------------=== main
int main(int argc, char **argv)
{
    if (argc > 2) get_flags(argc - 2, (const char **)(argv + 2));
    
    parse_expr(argv[1]);
    
    return 0;
}