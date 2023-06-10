#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* TYPE DEFENITIONS */
typedef signed char *isize;
typedef long long i64;
typedef long i32;
typedef int i16;
typedef char i8;

typedef unsigned char *usize;
typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned int u16;
typedef unsigned char u8;


/* FILE HANDLING */
long file_size(FILE *file) {
    if (!file) {
        return 0;
    }

    fpos_t original = 0;

    if (fgetpos(file, &original)) {
        printf("fgetpos() filed %i", errno);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long out = ftell(file);

    if (fsetpos(file, &original)) {
        printf("fsetpos() failed %i\n", errno);
    }

    return out;
}

char *file_contents(char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Could not open file at %s\n", path);
        return NULL;
    }

    long size = file_size(file);
    char *contents = malloc(size + 1);
    char *write_it = contents;
    size_t bytes_read = 0;
    size_t remaining_bytes = size;

    if (remaining_bytes > 0) {
        size_t brti = fread(write_it, 1, remaining_bytes, file);
        if (ferror(file)) {
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


void print_usage(char **argv) {
    printf("USAGE: %s <path_to_file_to_compile>", argv[0]);
}

/* ERRORS */
#define ERROR_CREATE(n, t, msg_c)   \
    Error(n) = {(t), (msg_c)}

#define ERROR_PREP(n, t, msg_p)     \
    (n).type = (t);                 \
    (n).msg = (msg_p);

typedef struct Error {
    enum ErrorType {
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

void print_error(Error err) {
    if (err.type == ERROR_NONE) {
        return;
    }
    assert(ERROR_MAX == 6);
    printf("ERROR: ");

    switch (err.type) {
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
    if (err.msg) {
        printf("\n\t-> %s", err.msg);
    }
}


/* LEXING */
#define nonep(node) ((node).type == NODE_TYPE_NONE)
#define integerp(node) ((node).type == NODE_TYPE_INTEGER)

const char *whitespace = " \r\n";
const char *delimiters = " \r\n,;/{}()<>"; 

// TODO:
// |-----API to create new node
// |-----API to add node as child
typedef struct Node {
    enum NodeType {
        NODE_TYPE_NONE,
        NODE_TYPE_INTEGER,
        NODE_TYPE_PROGRAM,
        NODE_TYPE_MAX,
    } type;
    union NodeValue {
        i32 integer;
    } value;
    struct Node **children;
} Node;

// TODO:
// |-----API to create new Binding
// |-----API to add Binding to Scope
typedef struct Binding {
    char *id;
    Node *value;
    struct Binding *next;
} Binding;

typedef struct Scope {
    struct Scope *parent;
    Binding *bind;
} Scope;

typedef struct Token
{
    char *beg;
    char *end;
    struct Token *next;
} Token;

int token_string_eq(char *string, Token *token) {
    if(!string||!token->beg||!token->end) return 0;
    char *beg = token->beg;
    while (*string && token->beg < token->end) {
        if (*string != *beg) return 0;
        string++;
        beg++;
    }
    return 1;
}

Token *token_create() {
    Token *token = malloc(sizeof(Token));
    assert(token && "Could not allocate memory token");
    memset(token, 0, sizeof(Token)); 
    return token;
}

void free_tokens(Token *root) {
    while (root) {
        Token *token_to_free = root;
        root = root->next;
        free(token_to_free);
    }
}

void print_tokens(Token *root) {
    size_t count = 1;
    while (root) {
        if (count > 10000) break; //FIXME
        printf("Token %zu: ", count);
        if (root->beg && root->end) {
            printf("%.*s", root->end - root->beg, root->beg);
        }
        putchar('\n');
        root = root->next; 
        count++;
    }
}

/* given a src (souce), get the next token, and point to it with beg (begin) & end (end)*/
Error lex(char *src, Token *token) {
    Error err = ok;
    if (!src || !token) {
        ERROR_PREP(err, ERROR_ARGUMENTS, "Can't lex empty source");
        return err;
    };

    token->beg = src;
    token->beg += strspn(token->beg, whitespace);
    token->end = token->beg;
    
    if (*(token->end) == '\0') {return err;}
    token->end += strcspn(token->beg, delimiters);
    if (token->end == token->beg) {token->end += 1;}
    return err;
}

void scope_set() {
}

Error parse_expr(char *src, Node *res) {
    Token *tokens = NULL;
    Token *token_iter = tokens;
    Token current_token;
    current_token.beg = src;
    current_token.end = src;
    current_token.next = NULL;
    
    Error err = ok;

    while ((err = lex(current_token.end, &current_token)).type == ERROR_NONE) {
        if (current_token.end - current_token.beg == 0) {
            break;
        }

        if(tokens) {
            token_iter->next = token_create();
            memcpy(token_iter->next, &current_token, sizeof(Token));
            token_iter = token_iter->next;
        } else {
            tokens = token_create();
            memcpy(tokens, &current_token, sizeof(Token));
            token_iter = tokens;
        }
    }

    print_tokens(tokens);
    
    Node *root = calloc(1, sizeof(Node));
    assert(root && "Could not allocate memory for AST Node");
    
    token_iter = tokens;

    while(token_iter) {
        // TODO: map constructs from the lanf and attempt to create nodes
        size_t token_length = token_iter->end - token_iter->beg;
        char *token_contents = malloc(token_length + 1);
        assert(token_contents && "Could not allocate string for token contents while parsing");
        memcpy(token_contents, token_iter->beg, token_length);
        token_contents[token_length] = '\0';

        if (token_string_eq("i32", token_iter)) {
            printf("Found 'i32' at token\n");

            if (token_string_eq("/", token_iter->next)) {
                if (token_string_eq("rw", token_iter->next->next)) {
                    printf("Found 'rw' at token\n");
                }
                else if (token_string_eq("r", token_iter->next->next)) {
                    printf("Found 'r' at token\n");
                }
            }
            
            if (token_iter->next && token_string_eq("=", token_iter->next->next)) {
                printf("Found assignment\n");
            } 
            
        }

        token_iter = token_iter->next;
    }

    free_tokens(tokens);

    return err;
}

/* MAIN */
int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv);
        exit(0);
    }

    char *path = argv[1];
    char *contents = file_contents(path);

    if (contents) {
        // printf("Contents of %s:\n---\n\"%s\"\n---\n", path, contents);
        Node *expression;
        print_error(parse_expr(contents, expression));
        free(contents);
    }

    return 0;
}