#include "../inc/scanner.h"

std::unordered_map<std::string, token_t> keyword_map =
    {
        {"/r", KW_PERM_R},
        {"/rx", KW_PERM_RX},
        {"/rs", KW_PERM_RS},
        {"/rw", KW_PERM_RW},
        {"/rwx", KW_PERM_RWX},
        {"/rws", KW_PERM_RWS},
        {"isize", KW_ISIZE},
        {"i32", KW_I32},
        {"usize", KW_USIZE},
        {"u32", KW_U32},
        {"f32", KW_F32},
        {"str", KW_STR},
        {"if", KW_IF},
        {"else", KW_ELSE},
        {"for", KW_FOR},
        {"while", KW_WHILE},
        {"return", KW_RETURN},

        {">=", COMB_GTOE},
        {"<=", COMB_LTOE},
        {"==", COMB_EQ},
        {"+=", COMB_MINUS_EQ},
        {"-=", COMB_PLUS_EQ},
        {"++", COMB_ADD},
        {"--", COMB_SUB},
        {"&&", COMB_AND},
        {"||", COMB_OR},

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

void skip_comments(auto &it, auto &pos, auto end)
{
    auto start = it++;
    while (it != end && *it != '\n')
    {
        ++it;
        ++pos.col;
    }

    pos.col += it - start;
}

void skip_whitespace(auto &it, auto &pos)
{
    if (*it == '\n')
    {
        pos.col = 1;
        ++pos.ln;
    }
    else
    {
        ++pos.col;
    }

    ++it;
}

auto is_digits(const std::string &str) -> bool
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

auto is_delim(auto &it) -> bool
{
    return (std::isspace(*it) || std::ispunct(*it)) && *it != '_';
}

void read_string(auto &tokens, auto &start, auto &it, auto &pos, auto &end)
{
    while (*(++it) != '"')
    {
        report(it == end, "Syntax Error: Unterminated string literal");
    }

    tokens.emplace_back(token{std::string(start, ++it), token_t::LIT_STR, pos});
    pos.col += it - start;
}

void read_keyword_with_delimeter(auto &start, auto &it)
{
    int iter = 4;
    auto tmp_str = std::string(start, it + iter);

    while (keyword_map.find(tmp_str) == keyword_map.end())
    {
        --iter;
        tmp_str.pop_back();
    }

    if (iter < 4)
    {
        it += iter;
    }
}

void get_tokens(auto &tokens, auto &it, auto &pos, auto &end)
{
    auto start = it;

    while (!is_delim(it) && it != end)
    {
        // read until delimeter
        ++it;
    }

    if (start == it)
    {
        // read delimiter
        ++it;
    }

    if (*start == '"')
    {
        read_string(tokens, start, it, pos, end);
        return;
    }
    else if (std::string("+-=></").find(*start) != std::string::npos)
    {
        read_keyword_with_delimeter(start, it);
    }

    auto lxm = std::string(start, it);
    auto type = keyword_map.find(lxm);

    if (is_digits(lxm))
    {
        tokens.emplace_back(token{lxm, token_t::LIT_NUM, pos});
    }
    else if (type == keyword_map.end())
    {
        tokens.emplace_back(token{lxm, token_t::IDENTIFIER, pos});
    }
    else
    {
        tokens.emplace_back(token{lxm, type->second, pos});
    }

    pos.col += it - start;
}

auto scan(const std::string &path) -> std::vector<token>
{
    auto buffer = read_file(path);
    report(buffer->empty(), "Can't parse an empty source");

    std::vector<token> tokens = {};
    position pos = {.ln = 1, .col = 1};

    for (auto it = buffer->begin(); it != buffer->end();)
    {
        if (*it == '#')
        {
            skip_comments(it, pos, buffer->end());
        }
        else if (!isspace(*it))
        {
            get_tokens(tokens, it, pos, buffer->end());
        }
        else
        {
            skip_whitespace(it, pos);
        }
    }

    return tokens;
}