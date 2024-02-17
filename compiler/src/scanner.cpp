#include "../inc/scanner.h"

std::unordered_map<std::string, token_t> keyword_map = {
    {"/r", KW_PERM_R},
    {"/rx", KW_PERM_RX},
    {"/rs", KW_PERM_RS},
    {"/rw", KW_PERM_RW},
    {"/rwx", KW_PERM_RWX},
    {"/rws", KW_PERM_RWS},

    {"bool", KW_TYPE_BOOL},
    {"isize", KW_TYPE_ISIZE},
    {"i32", KW_TYPE_I32},
    {"usize", KW_TYPE_USIZE},
    {"u32", KW_TYPE_U32},
    {"f32", KW_TYPE_F32},
    {"str", KW_TYPE_STR},

    {"fn", KW_FN},
    {"mod", KW_MOD},
    {"if", KW_IF},
    {"else", KW_ELSE},
    {"true", KW_TRUE},
    {"false", KW_FALSE},
    {"for", KW_FOR},
    {"while", KW_WHILE},
    {"return", KW_RETURN},

    {"::", COMB_ACCESS},
    {">=", COMB_GTOE},
    {"<=", COMB_LTOE},
    {"==", COMB_EQ},
    {"!=", COMB_NEQ},
    {"->", COMB_RETURN},
    {"+=", COMB_MINUS_EQ},
    {"-=", COMB_PLUS_EQ},
    {"++", COMB_ADD},
    {"--", COMB_SUB},
    {"&&", COMB_AND},
    {"||", COMB_OR},

    {"@", PUNCT_AT},
    {"/", PUNCT_FW_SLASH},
    {"\\", PUNCT_BC_SLASH},
    {"!", PUNCT_EXCLM},
    {"#", PUNCT_HTAG},
    {"%", PUNCT_PRCNT},
    {"&", PUNCT_AMPRSND},
    {"*", PUNCT_ASTER},
    {"(", PUNCT_L_PAREN},
    {")", PUNCT_R_PAREN},
    {"{", PUNCT_L_SQUIG},
    {"}", PUNCT_R_SQUIG},
    {"[", PUNCT_L_SQRBR},
    {"]", PUNCT_R_SQRBR},
    {"'", PUNCT_SQUOTE},
    {"\"", PUNCT_DQUOTE},
    {";", PUNCT_SEMI},
    {":", PUNCT_COL},
    {">", PUNCT_GT},
    {"<", PUNCT_LT},
    {"=", PUNCT_EQ},
    {"-", PUNCT_MINUS},
    {"+", PUNCT_PLUS},
    {".", PUNCT_POINT},
    {",", PUNCT_COMMA},
};

auto is_digits(const std::string &str) -> bool
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

auto get_type(const std::string &str) -> token_t
{
    return keyword_map.count(str) ? keyword_map.at(str) : token_t::IDENTIFIER;
}

void skip_comments(file_reader &fr)
{
    while (fr.peek() != EOF && !fr.equals('\n'))
    {
        fr.advance();
    }
}

void skip_whitespace(file_reader &fr, position &pos)
{
    if (fr.equals('\n'))
    {
        pos.col = 1;
        ++pos.ln;
    }
    else
    {
        ++pos.col;
    }

    fr.advance();
}

void add_token(std::vector<token> &tokens, const std::string &str, position &pos, token_t type = token_t::UNDEFINED)
{
    tokens.emplace_back(
        token{
            str,
            type != token_t::UNDEFINED ? type : get_type(str),
            pos});

    pos.col += str.size();
}

void get_id(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos)
{
    while ((fr.is_alnum() || fr.equals('_')) && !fr.at_eof())
    {
        fr.write_to(str);
    }

    add_token(tokens, str, pos);
}

void get_number_lit(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos)
{
    auto punct_used = 0;
    while ((fr.is_digit() || fr.equals('.')) && !fr.at_eof() && punct_used <= 1)
    {
        if (fr.equals('.'))
        {
            punct_used++;
        }

        fr.write_to(str);
    }

    add_token(tokens, str, pos, token_t::LIT_NUM);
}

void get_string_lit(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos)
{
    fr.write_to(str);

    while (!fr.equals('\"'))
    {
        error(fr.at_eof(), "String literal is missing a closing sign");
        fr.write_to(str);
    }

    fr.write_to(str);

    add_token(tokens, str.substr(1, str.size() - 2), pos, token_t::LIT_STR);
}

void get_combo_token(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos)
{
    fr.write_to(str);
    if (fr.is_punct())
    {
        fr.write_to(str);

        auto type = keyword_map.find(str);

        if (type == keyword_map.end())
        {
            add_token(tokens, str.substr(0, 1), pos);
            add_token(tokens, str.erase(0, 1), pos);
        }
        else
        {
            add_token(tokens, str, pos);
        }
    }
    else
    {
        add_token(tokens, str, pos);
    }
}

void get_permission_token(file_reader &fr, std::vector<token> &tokens, std::string &str, position &pos)
{
    while (!fr.at_eof() && !fr.is_space())
    {
        fr.write_to(str);

        if (fr.is_punct()) {
            break;
        }
    }

    auto type = keyword_map.find(str);

    if (type == keyword_map.end())
    {
        add_token(tokens, str.substr(0, 1), pos, token_t::PUNCT_FW_SLASH);
        add_token(tokens, str.erase(0, 1), pos);
    }
    else
    {
        add_token(tokens, str, pos, type->second);
    }
}

void get_token(std::vector<token> &tokens, file_reader &fr, position &pos)
{
    auto str = std::string();

    // get identifier
    if (fr.equals('_') || fr.is_alpha())
    {
        get_id(fr, tokens, str, pos);
    }
    else if (fr.is_digit())
    {
        get_number_lit(fr, tokens, str, pos);
    }
    else if (fr.equals('\"'))
    {
        get_string_lit(fr, tokens, str, pos);
    }
    else if (fr.equals('/'))
    {
        get_permission_token(fr, tokens, str, pos);
    }
    else if (fr.is_punct())
    {
        get_combo_token(fr, tokens, str, pos);
    }
    else
    {
        fr.write_to(str);
        add_token(tokens, str, pos);
    }
}

auto scan(const std::string &path) -> std::vector<token>
{
    file_reader fr = file_reader(path, 0);
    position pos = {.ln = 1, .col = 1};
    std::vector<token> tokens = {};

    fr.advance();

    while (!fr.at_eof())
    {
        if (fr.equals('#'))
        {
            skip_comments(fr);
        }
        else if (fr.is_space())
        {
            skip_whitespace(fr, pos);
        }
        else
        {
            get_token(tokens, fr, pos);
        }
    }

    return tokens;
}
