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
static const char *delimiters = " \r\n(){}[]+-/*=,.;";

// ===-------------------------------------------=== STRUCTS
typedef enum token_t
{
    undefined,
    keyword,
    identifier,
    literal,
    punctuation
} token_t;

typedef struct Token
{
    char *lexeme;
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
char *assign_lexeme(const char *_src, size_t _src_size) {
    char *lexeme = (char *)malloc(_src_size);
    memcpy(lexeme, _src, _src_size);
    *(lexeme + _src_size) = '\0'; 
    return lexeme;
}

Token *create_token(char *_src, size_t _src_size)
{

    Token *new_tok = calloc(1, sizeof(Token));

    if (!new_tok) perroex("couldn't allocate memory for a new token");

    new_tok->lexeme = assign_lexeme((const char *) _src, _src_size);

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
            perroex("can't lex an empty source");

        beg += strspn(beg, whitespace);
        end = beg + strcspn(beg, delimiters);

        if (end - beg == 0)
        {
            end++;
        }

        if (!head_node) 
        { 
            head_node = create_token(beg, end - beg);
            current_node = head_node;
        } else 
        {
            current_node->next = create_token(beg, end - beg); 
            current_node = current_node->next;
        }

        beg = end;
    }

    return head_node;
}

// ===-------------------------------------------=== parser
void init_scanner(char *_fpath)
{
    char *buff = readf_2buff(_fpath);
    Token *tok_list = lex(buff);
    while (tok_list) { puts(tok_list->lexeme); tok_list = tok_list->next;}
}


void get_command_flags(const int _flagc, const char **flagv)
{

    for (int i = 0; i < _flagc; ++i)
    {
        if (!strcmp(flagv[i], "--lean-ron"))
            print_lean_ronin();
        else if (!strcmp(flagv[i], "--uni-ron"))
            print_universal_ronin();
    }
}

// ===-------------------------------------------=== main
int main(int argc, char **argv)
{
    if (argc > 2)
        get_command_flags(argc - 2, (const char **)(argv + 2));

    init_scanner(argv[1]);

    return 0;
}