#include <stdlib.h>
#include <stdio.h>
#include "ANSI.h"

enum tok_type {
    tok_keyword, 
    tok_operator,
    tok_literal,
    tok_identifier
};

typedef struct token {
    enum tok_type type;
    char *lexeme;
    long line;
    int col;
} token;

char *readf_2buff(char *_fpath)
{
    if (!_fpath)
    {
        printf(RC_Red "\n\t --> error: " Reset "no input files\n\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(_fpath, "rb");

    // get file size
    fseek(fp, 0, SEEK_END);
    unsigned long fs = ftell(fp);
    rewind(fp);

    char *buff = (char *)malloc(sizeof(char) * (fs + 1));

    if (!buff)
    {
        printf(RC_Red "\n\t --> error: " Reset "out of memory\n\n");
        exit(EXIT_FAILURE);
    }

    if (fread(buff, 1, fs, fp) != fs)
    {
        printf(RC_Red "\n\t --> error: " Reset "eof reached while reading\n\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    buff[fs] = '\0';

    return buff;
}

void lex(const char *src)
{
       
}

void parse_expr(char *_fpath)
{
    char *buff = readf_2buff(_fpath);
    lex(buff);
}

int main(int argc, char **argv)
{
    // printf("%s", parsef(argv[1]));
    parse_expr(argv[1]);
    return 0;
}