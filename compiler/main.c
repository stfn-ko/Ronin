#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

long file_size(FILE *file)
{
    if (!file)
    {
        return 0;
    }

    fpos_t original = 0;

    if (fgetpos(file, &original))
    {
        printf("fgetpos() filed %i", errno);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long out = ftell(file);

    if (fsetpos(file, &original))
    {
        printf("fsetpos() failed %i\n", errno);
    }

    return out;
}

char *file_contents(char *path)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        printf("Could not open file at %s\n", path);
        return NULL;
    }

    long size = file_size(file);
    char *contents = malloc(size + 1);
    char *write_it = contents;
    size_t bytes_read = 0;

    while (bytes_read < size)
    {
        // bytes read this iteration
        size_t brti = fread(write_it, 1, size - bytes_read, file);
        if (ferror(file))
        {
            printf("Error while reading: %i\n", errno);
            free(contents);
            return NULL;
        }

        bytes_read += brti;
        write_it += brti;

        if (feof(file))
        {
            break;
        }
    }

    contents[bytes_read] = '\0';
    return contents;
}

void print_usage(char **argv)
{
    printf("USAGE: %s <path_to_file_to_compile>", argv[0]);
}

/* ERRORS */
typedef struct Error
{
    enum ErrorType
    {
        ERROR_NONE = 0,
        ERROR_ARGUMENTS,
        ERROR_TYPE,
        ERROR_GENERIC,
        ERROR_SYNTAX,
        ERROR_TODO,
    } type;
    char *msg;
} Error;

Error ok = {ERROR_NONE, NULL};

void print_error(Error err)
{
    if (err.type == ERROR_NONE)
    {
        return;
    }

    printf("ERROR: ");
    switch (err.type)
    {
    default:
        printf("Unknown error type...");
        break;
    case ERROR_TODO:
        printf("TODO (not implemented)");
        break;
    case ERROR_SYNTAX:
        printf("Invalid syntax");
        break;
    case ERROR_TYPE:
        printf("Mismatched types");
        break;
    case ERROR_ARGUMENTS:
        printf("Invalid arguments");
        break;
    case ERROR_NONE:
    case ERROR_GENERIC:
        break;
    }

    if (err.msg)
    {
        printf("\n\t-> %s", err.msg);
    }
}

#define ERROR_CREATE(n, t, msg_c) \
    Error(n) = {(t), (msg_c)}

#define ERROR_PREP(n, t, msg_p) \
    (n).type = (t);             \
    (n).msg = (msg_p);

/* LEXING */
const char *whitespace = " \r\n";
const char *delimiters = " \r\n<";

/* given a src (souce), get the next token, and point to it with beg (begin) & end (end)*/
Error lex(char *src, char **beg, char **end)
{
    Error err = ok;

    if (!src || !beg || !end)
    {
        ERROR_PREP(err, ERROR_ARGUMENTS, "Can't lex empty source");
        return err;
    };

    *beg = src;
    *beg += strspn(*beg, whitespace);
    *end = *beg;
    *end += strcspn(*beg, delimiters);
    return err;
}

Error parse(char *src)
{
    Error err = ok;
    char *beg = src;
    char *end = src;

    while ((err = lex(end, &beg, &end)).type == ERROR_NONE)
    {
        if (end - beg == 0)
        {
            break;
        }
        printf("lexed: %.*s\n", end - beg, beg);
    }

    return err;
}

/* MAIN */
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        print_usage(argv);
        exit(0);
    }

    char *path = argv[1];
    char *contents = file_contents(path);

    if (contents)
    {
        // printf("Contents of %s:\n---\n\"%s\"\n---\n", path, contents);
        print_error(parse(contents));
        free(contents);
    }

    return 0;
}