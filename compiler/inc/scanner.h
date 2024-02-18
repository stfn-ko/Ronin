#pragma once

#include <vector>
#include <unordered_map>

#include "error_handler.h"
#include "file_handler.h"

typedef enum
{
    UNDEFINED,
    IDENTIFIER,

    KW_PERM_R,
    KW_PERM_RX,
    KW_PERM_RS,

    KW_PERM_RW,
    KW_PERM_RWX,
    KW_PERM_RWS,

    KW_TYPE_BOOL,
    KW_TYPE_ISIZE,
    KW_TYPE_I32,
    KW_TYPE_USIZE,
    KW_TYPE_U32,
    KW_TYPE_F32,
    KW_TYPE_STR,

    KW_FN,
    KW_MOD,
    KW_IF,
    KW_ELSE,
    KW_TRUE,
    KW_FALSE,
    KW_FOR,
    KW_WHILE,
    KW_RETURN,

    //

    COMB_ACCESS,
    COMB_GTOE,
    COMB_LTOE,
    COMB_EQ,
    COMB_NEQ,
    COMB_RETURN,
    COMB_MINUS_EQ,
    COMB_PLUS_EQ,
    COMB_ADD,
    COMB_SUB,
    COMB_AND,
    COMB_OR,

    //

    LIT_NUM,
    LIT_STR,

    //
    PUNCT_AT,
    PUNCT_FW_SLASH,
    PUNCT_BC_SLASH,
    PUNCT_EXCLM,
    PUNCT_HTAG,
    PUNCT_PRCNT,
    PUNCT_AMPRSND,
    PUNCT_ASTER,
    PUNCT_L_PAREN,
    PUNCT_R_PAREN,
    PUNCT_L_SQUIG,
    PUNCT_R_SQUIG,
    PUNCT_L_SQRBR,
    PUNCT_R_SQRBR,
    PUNCT_SQUOTE,
    PUNCT_DQUOTE,
    PUNCT_GT,
    PUNCT_LT,
    PUNCT_EQ,
    PUNCT_SEMI,
    PUNCT_COL,
    PUNCT_MINUS,
    PUNCT_PLUS,
    PUNCT_POINT,
    PUNCT_COMMA,
} token_t;

typedef struct
{
    std::string lexeme;
    token_t type;
    position pos;
} token;

class file_reader
{
private:
    FILE *self;
    int iter;

public:
    file_reader()
    {
        this->self = nullptr;
        this->iter = 0;
    }

    file_reader(const std::string &_path, int _iter)
    {
        this->self = fopen(_path.c_str(), "r");
        error(this->self == NULL, "Error opening file");

        this->iter = _iter;
    }

    ~file_reader()
    {
        fclose(this->self);
    }

    // Automatically increments the iterator and appends the character to the given string
    void write_to(std::string &s)
    {
        s.push_back(this->iter);
        this->advance();
    }

    // Advance the iterator and return the updated value
    auto advance() -> int { return this->iter = getc(this->self); }

    // Check if the current character equals the provided character
    auto equals(char c) -> bool { return (this->iter == c); }

    // Check if the current character is an alphabetic character
    auto is_alpha() -> bool { return isalpha(this->iter); }

    // Check if the current character is a punctuation character
    auto is_punct() -> bool { return ispunct(this->iter); }

    // Check if the current character is a whitespace character
    auto is_space() -> bool { return isspace(this->iter); }

    // Check if the current character is a digit
    auto is_digit() -> bool { return isdigit(this->iter); }

    // Check if the current character is alphanumeric
    auto is_alnum() -> bool { return isalnum(this->iter); }

    // Check if the iterator is at the end of the file
    auto at_eof() -> bool { return (this->iter == EOF); }

    // Return the current character without advancing the iterator
    auto peek() -> int { return this->iter; }
};

auto is_digits(const std::string &str) -> bool;

void skip_comments(file_reader &fr);
void skip_whitespace(file_reader &fr, position &pos);

void get_number_lit(file_reader &fr, std::vector<token> &tokens, position &pos);
void get_string_lit(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos);
void get_combo_token(file_reader &fr, std::vector<token> &tokens, position &pos);
void get_permission_token(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos);

void get_token(std::vector<token> &tokens, file_reader &fr, position &pos);
auto scan(const std::string &path) -> std::vector<token>;