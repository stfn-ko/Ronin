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
    MISC_AT,
    MISC_FW_SLASH,
    MISC_BC_SLASH,
    MISC_EXCLM,
    MISC_HTAG,
    MISC_PRCNT,
    MISC_AMPRSND,
    MISC_STAR,
    MISC_L_PAREN,
    MISC_R_PAREN,
    MISC_L_SQUIG,
    MISC_R_SQUIG,
    MISC_L_SQRBR,
    MISC_R_SQRBR,
    MISC_SQUOTE,
    MISC_DQUOTE,
    MISC_GT,
    MISC_LT,
    MISC_EQ,
    MISC_SEMI,
    MISC_COL,
    MISC_MINUS,
    MISC_PLUS,
    MISC_POINT,
    MISC_COMMA,
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

    void write_to(std::string &s)
    {
        s.push_back(this->iter);
        this->advance();
    }

    auto advance() -> int { return this->iter = getc(this->self); }

    auto equals(char c) -> bool { return (this->iter == c); }

    auto is_alpha() -> bool { return isalpha(this->iter); }

    auto is_punct() -> bool { return ispunct(this->iter); }

    auto is_space() -> bool { return isspace(this->iter); }

    auto is_digit() -> bool { return isdigit(this->iter); }

    auto is_alnum() -> bool { return isalnum(this->iter); }

    auto at_eof() -> bool { return (this->iter == EOF); }

    auto peek() -> int { return this->iter; }
};

auto is_digits(const std::string &str) -> bool;

void skip_comments(file_reader &fr);
void skip_whitespace(file_reader &fr, position &pos);

void get_number_lit(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos);
void get_string_lit(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos);
void get_combo_token(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos);
void get_permission_token(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos);

void get_token(std::vector<token> &tokens, file_reader &fr, position &pos);
auto scan(const std::string &path) -> std::vector<token>;