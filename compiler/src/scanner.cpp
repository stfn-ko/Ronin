#include "../inc/scanner.h"

std::unordered_map<std::string, token_t> keyword_map =
    {
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
        {"for", KW_FOR},
        {"while", KW_WHILE},
        {"return", KW_RETURN},

        {"::", COMB_ACCESS},
        {">=", COMB_GTOE},
        {"<=", COMB_LTOE},
        {"==", COMB_EQ},
        {"->", COMB_RETURN},
        {"+=", COMB_MINUS_EQ},
        {"-=", COMB_PLUS_EQ},
        {"++", COMB_ADD},
        {"--", COMB_SUB},
        {"&&", COMB_AND},
        {"||", COMB_OR},

        {"@", MISC_AT},
        {"/", MISC_FW_SLASH},
        {"\\", MISC_BC_SLASH},
        {"!", MISC_EXCLM},
        {"#", MISC_HTAG},
        {"%", MISC_PRCNT},
        {"&", MISC_AMPRSND},
        {"*", MISC_STAR},
        {"(", MISC_L_PAREN},
        {")", MISC_R_PAREN},
        {"{", MISC_L_SQUIG},
        {"}", MISC_R_SQUIG},
        {"[", MISC_L_SQRBR},
        {"]", MISC_R_SQRBR},
        {"'", MISC_SQUOTE},
        {"\"", MISC_DQUOTE},
        {";", MISC_SEMI},
        {":", MISC_COL},
        {">", MISC_GT},
        {"<", MISC_LT},
        {"=", MISC_EQ},
        {"-", MISC_MINUS},
        {"+", MISC_PLUS},
        {".", MISC_POINT},
        {",", MISC_COMMA},
};

auto is_digits(const std::string &str) -> bool
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

struct file_reader
{
    FILE *self;
    int iter;
};

auto get_next(file_reader &fr) -> int
{
    return fr.iter = getc(fr.self);
}

auto get_type(std::string &str) -> token_t
{
    return keyword_map.count(str) ? keyword_map.at(str) : token_t::IDENTIFIER;
}

void skip_comments(file_reader &fr)
{
    while (fr.iter != EOF && fr.iter != '\n')
    {
        get_next(fr);
    }
}

void skip_whitespace(file_reader &fr, position &pos)
{
    if (fr.iter == '\n')
    {
        pos.col = 1;
        ++pos.ln;
    }
    else
    {
        ++pos.col;
    }

    get_next(fr);
}

void get_token(std::vector<token> &tokens, file_reader &fr, position &pos)
{
    auto str = std::string();

    // get identifier
    if (fr.iter == '_' || isalpha(fr.iter))
    {
        while ((isalnum(fr.iter) || fr.iter == '_') && fr.iter != EOF)
        {
            str.push_back(fr.iter);
            get_next(fr);
        }
    }
    // get number literal
    else if (isdigit(fr.iter))
    {
        auto punct_used = 0;
        while ((isdigit(fr.iter) || fr.iter == '.') && fr.iter != EOF && punct_used <= 1)
        {
            if (fr.iter == '.')
            {
                punct_used++;
            }

            str.push_back(fr.iter);
            get_next(fr);
        }

        tokens.emplace_back(token{str, token_t::LIT_NUM, pos});
        pos.col += str.size();
        return;
    }
    else if (fr.iter == '\"')
    {
        str.push_back(fr.iter);
        get_next(fr);

        while (fr.iter != '\"')
        {
            error(fr.iter == EOF, "String literal is missing a closing sign");
            str.push_back(fr.iter);
            get_next(fr);
        }

        str.push_back(fr.iter);
        get_next(fr);

        tokens.emplace_back(token{str, token_t::LIT_STR, pos});
        pos.col += str.size();
        return;
    }
    else if (fr.iter == '/')
    {
        while (fr.iter != EOF && !isspace(fr.iter))
        {
            str.push_back(fr.iter);
            get_next(fr);
        }

        auto type = keyword_map.find(str);

        if (type == keyword_map.end())
        {
            tokens.emplace_back(token{str.substr(0, 1), token_t::MISC_FW_SLASH, pos});
            tokens.emplace_back(token{str.substr(1), token_t::UNDEFINED, pos});
        }
        else
        {
            tokens.emplace_back(token{str, type->second, pos});
        }

        return;
    }
    // handle the fucking combo tokens
    else
    {
        str.push_back(fr.iter);
        get_next(fr);
    }

    tokens.emplace_back(token{str, get_type(str), pos});
    pos.col += str.size();
    return;
}

auto scan(const std::string &path) -> std::vector<token>
{
    file_reader fr = {fopen(path.c_str(), "r"), 0};
    get_next(fr);

    error(fr.self == NULL, "Error opening file");

    std::vector<token> tokens = {};
    position pos = {.ln = 1, .col = 1};

    while (fr.iter != EOF)
    {
        if (fr.iter == '#')
        {
            skip_comments(fr);
        }
        else if (isspace(fr.iter))
        {
            skip_whitespace(fr, pos);
        }
        else
        {
            get_token(tokens, fr, pos);
        }
    }

    fclose(fr.self);
    return tokens;
}
