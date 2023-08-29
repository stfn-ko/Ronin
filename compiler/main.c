#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ANSI.h"

enum tok_type
{
    tok_keyword,
    tok_identifier,
    tok_literal,
    tok_punctuation,
    tok_delimeter
};

typedef struct token
{
    enum tok_type type;
    char *lexeme;
    int line;
    int col;
} token;

char *readf_2buff(char *_fpath)
{
    if (!_fpath)
    {
        printf(UL_Red "\n\terror -->" Reset " no input files\n\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(_fpath, "rb");

    if (!fp)
    {
        printf(UL_Red "\n\terror -->" Reset " no such file or directory\n\n");
        exit(EXIT_FAILURE);
    }

    if (!strstr(_fpath, ".ro"))
    {
        printf(UL_Red "\n\terror --> invalid file extension\n\n" Reset);
        exit(EXIT_FAILURE);
    }

    // get file size
    fseek(fp, 0, SEEK_END);
    unsigned long fs = ftell(fp);
    rewind(fp);

    char *buff = (char *)malloc(sizeof(char) * (fs + 1));

    if (!buff)
    {
        printf(UL_Red "\n\terror -->" Reset " out of memory\n\n");
        exit(EXIT_FAILURE);
    }

    if (fread(buff, 1, fs, fp) != fs)
    {
        printf(UL_Red "\n\terror -->" Reset " eof reached while reading\n\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    buff[fs] = '\0';

    return buff;
}

static const char *whitespace = " \r\n";
static const char *delimiters = " \r\n(){}[]";
static const char *punctuators = "+-/*=,.;";

size_t min_strcspn(char *_cptr, const char *_cstr1, const char *_cstr2)
{
    size_t c1 = strcspn(_cptr, _cstr1);
    size_t c2 = strcspn(_cptr, _cstr2);
    return (c1 < c2) ? c1 : c2;
}

void *lex(char *_src)
{
    char *beg = _src;
    char *end = _src;

    while (*end != '\0')
    {
        if (!_src || !beg || !end)
        {
            printf(UL_Red "\n\terror -->" Reset " can't lex an empty source\n\n");
            exit(EXIT_FAILURE);
        }

        beg += strspn(beg, whitespace);
        end = beg + min_strcspn(beg, delimiters, punctuators);

        if (end - beg == 0)
            end++;

        // assignment here
        printf("lexed: %.*s\n", end - beg, beg);

        beg = end;
    }
}

void parse_expr(char *_fpath)
{
    char *buff = readf_2buff(_fpath);
    lex(buff);
}

int main(int argc, char **argv)
{
    parse_expr(argv[1]);
    return 0;
}