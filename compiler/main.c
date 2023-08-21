////////////////////////////////////////////    /* INCLUDES */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////

////////////////////////////////////////////    /* FILE HANDLING */
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
    size_t remaining_bytes = size;

    if (remaining_bytes > 0)
    {
        size_t brti = fread(write_it, 1, remaining_bytes, file);
        if (ferror(file))
        {
            printf("Error while reading: %i\n", errno);
            free(contents);
            return NULL;
        }
        bytes_read += brti;
        write_it += brti;
        remaining_bytes -= brti;
    }

    contents[bytes_read] = '\0';
    return contents;
}

void print_usage(char **argv)
{
    printf("USAGE: %s <path_to_file_to_compile>", argv[0]);
}
////////////////////////////////////////////

////////////////////////////////////////////    /* ERRORS */
#define ERROR_CREATE(n, t, msg_c) \
    Error(n) = {(t), (msg_c)}

#define ERROR_PREP(n, t, msg_p) \
    (n).type = (t);             \
    (n).msg = (msg_p);

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
        ERROR_MAX,
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
    assert(ERROR_MAX == 6);
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
////////////////////////////////////////////

////////////////////////////////////////////    /* LEXING */
#define nonep(node) ((node).type == NODE_TYPE_NONE)
#define integerp(node) ((node).type == NODE_TYPE_INTEGER)

const char *whitespace = " \r\n";
const char *delimiters = " \r\n,;/{}()<>";

typedef struct Token
{
    char *beg;
    char *end;
    struct Token *next;
} Token;

/* given a src (souce), get the next token, and point to it with beg (begin) & end (end)*/
Error lex(char *src, Token *token)
{
    Error err = ok;
    if (!src || !token)
    {
        ERROR_PREP(err, ERROR_ARGUMENTS, "Can't lex empty source");
        return err;
    };

    token->beg = src;
    token->beg += strspn(token->beg, whitespace);
    token->end = token->beg;

    if (*(token->end) == '\0')
    {
        return err;
    }
    token->end += strcspn(token->beg, delimiters);
    if (token->end == token->beg)
    {
        token->end += 1;
    }
    return err;
}
////////////////////////////////////////////

////////////////////////////////////////////    /* PARSING */
// TODO:
// |-----API to create new node
// |-----API to add node as child
typedef struct Node
{
    enum NodeType
    {
        NODE_TYPE_NONE,
        NODE_TYPE_INTEGER,
        NODE_TYPE_PROGRAM,
        NODE_TYPE_MAX,
    } type;
    union NodeValue
    {
        __INT64_TYPE__ integer;
    } value;
    struct Node *children;
    struct Node *next_child;
} Node;

// TODO:
// |-----API to create new Binding
// |-----API to add Binding to Scope
typedef struct Binding
{
    char *id;
    Node *value;
    struct Binding *next;
} Binding;

typedef struct Scope
{
    struct Scope *parent;
    Binding *bind;
} Scope;

int token_string_eq(char *string, Token *token)
{
    if (!string || !token->beg || !token->end)
        return 0;
    char *beg = token->beg;
    while (*string && token->beg < token->end)
    {
        if (*string != *beg)
            return 0;
        string++;
        beg++;
    }
    return 1;
}

Token *token_create()
{
    Token *token = malloc(sizeof(Token));
    assert(token && "Could not allocate memory token");
    memset(token, 0, sizeof(Token));
    return token;
}

void free_token(Token *root)
{
    while (root)
    {
        Token *token_to_free = root;
        root = root->next;
        free(token_to_free);
    }
}

// TODO: Make more efficient
void free_node(Node *root)
{
    if (!root)
    {
        return;
    }

    Node *child = root->children;
    Node *tmp = child->next_child;

    while (child)
    {
        tmp = child->next_child;
        free_node(child);
        child = tmp;
    }

    free(root);
}

void print_tokens(Token *root)
{
    size_t count = 1;
    while (root)
    {
        if (count > 10000)
        {
            break; // FIXME {remove this limit}
        }

        printf("Token %zu: ", count);

        if (root->beg && root->end)
        {
            printf("%.*s\n", root->end - root->beg, root->beg);
        }

        root = root->next;

        count++;
    }
}

Error parse_expr(char *src, Node *res)
{
    Error err = ok;
    const size_t lookback_size = 8;

    Token current_token;
    Token *tokens = NULL;
    Token *tokens_it = tokens;
    Token lookback[lookback_size];

    current_token.beg = src;
    current_token.end = src;
    current_token.next = NULL;

    Node *root = calloc(1, sizeof(Node));
    assert(root && "Could not allocate memory for AST Root.");

    Node current_node;some foobar
    current_node.children = NULL;
    current_node.next_child = NULL;
    current_node.type = NODE_TYPE_NONE;
    current_node.value.i32 = 0;

    while ((err = lex(current_token.end, &current_token)).type == ERROR_NONE)
    {
        if (current_token.end - current_token.beg == 0)
        {
            break;
        }

        if (token_string_eq(""))

        for (size_t i = 1; i < lookback_size; ++i)
        {
            lookback[i-1] = lookback[i];
        }

        memcpy(&lookback[lookback_size - 1], &current_token, sizeof(Token));
                
    }

    return err;
}

void scope_set()
{
}
////////////////////////////////////////////

////////////////////////////////////////////    /* MAIN */
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
        Node *expression;
        print_error(parse_expr(contents, expression));
        free(contents);
    }

    return 0;
}