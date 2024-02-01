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

    KW_TYPE_ISIZE,
    KW_TYPE_I32,
    KW_TYPE_USIZE,
    KW_TYPE_U32,
    KW_TYPE_F32,
    KW_TYPE_STR,

    KW_FN,

    KW_IF,
    KW_ELSE,

    KW_FOR,
    KW_WHILE,

    KW_RETURN,

    //

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

auto is_digits(const std::string &str) -> bool;
auto is_delim(auto &it) -> bool;

void skip_comments(auto &it, auto &pos, auto end);
void skip_whitespace(auto &it, auto &pos);

void read_string(auto &tokens, auto &start, auto &it, auto &pos, auto &end);
void read_keyword_with_delimeter(auto &start, auto &it);

void get_tokens(auto &tokens, auto &it, auto &pos, auto end);
auto scan(const std::string &path) -> std::vector<token>;