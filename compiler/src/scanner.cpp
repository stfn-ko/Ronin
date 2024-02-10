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
        {"if", KW_IF},
        {"else", KW_ELSE},
        {"for", KW_FOR},
        {"while", KW_WHILE},
        {"return", KW_RETURN},

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

// void skip_comments(auto &it, auto &pos, auto end)
// {
//     auto start = it++;
//     while (it != end && *it != '\n')
//     {
//         ++it;
//         ++pos.col;
//     }
// }

// void skip_whitespace(auto &it, auto &pos)
// {
//     if (*it == '\n')
//     {
//         pos.col = 1;
//         ++pos.ln;
//     }
//     else
//     {
//         ++pos.col;
//     }

//     ++it;
// }

auto is_digits(const std::string &str) -> bool
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

// auto is_delim(auto &it) -> bool
// {
//     return (std::isspace(*it) || std::ispunct(*it)) && *it != '_';
// }

// void read_string(auto &tokens, auto &start, auto &it, auto &pos, auto &end)
// {
//     while (*(++it) != '"')
//     {
//         error(it == end, "Unterminated string literal");
//     }

//     tokens.emplace_back(token{std::string(start, ++it), token_t::LIT_STR, pos});
//     pos.col += it - start;
// }

// void read_keyword_with_delimeter(auto &start, auto &it)
// {
//     int iter = 4;
//     auto tmp_str = std::string(start, it + iter);

//     while (keyword_map.find(tmp_str) == keyword_map.end())
//     {
//         --iter;
//         tmp_str.pop_back();
//     }

//     if (iter < 4)
//     {
//         it += iter;
//     }
// }

// void get_tokens(auto &tokens, auto &it, auto &pos, auto end)
// {
//     auto start = it;

//     if (*it == '_' || isalpha(*it))
//     {
//         // get identifiers
//         while ((isalnum(*it) || *it == '_') && it != end)
//         {
//             ++it;
//         }
//     }

//     else if (isdigit(*it))
//     {
//         auto punct_used = 0;
//         // get number literal
//         while ((isdigit(*it) || *it == '.') && it != end && punct_used <= 1)
//         {
//             if (*it == '.')
//             {
//                 punct_used++;
//             }

//             it++;
//         }
//     }

//     if (start == it)
//     {
//         // read delimiter
//         ++it;
//     }

//     if (*start == '"')
//     {
//         read_string(tokens, start, it, pos, end);
//         return;
//     }
//     else if (std::string("+-=></").find(*start) != std::string::npos)
//     {
//         read_keyword_with_delimeter(start, it);
//     }

//     auto lxm = std::string(start, it);
//     auto type = keyword_map.find(lxm);

//     if (is_digits(lxm))
//     {
//         tokens.emplace_back(token{lxm, token_t::LIT_NUM, pos});
//     }
//     else if (type == keyword_map.end())
//     {
//         tokens.emplace_back(token{lxm, token_t::IDENTIFIER, pos});
//     }
//     else
//     {
//         tokens.emplace_back(token{lxm, type->second, pos});
//     }

//     pos.col += it - start;
// }

// auto scan(const std::string &path) -> std::vector<token>
// {
//     auto buffer = read_file(path);
//     error(buffer->empty(), "Can't parse an empty source");

//     std::vector<token> tokens = {};
//     position pos = {.ln = 1, .col = 1};

//     for (auto it = buffer->begin(); it != buffer->end();)
//     {
//         if (*it == '#')
//         {
//             skip_comments(it, pos, buffer->end());
//         }
//         else if (!isspace(*it))
//         {
//             get_tokens(tokens, it, pos, buffer->end());
//         }
//         else
//         {
//             skip_whitespace(it, pos);
//         }
//     }

//     return tokens;
// }

struct file_reader
{
    FILE *self;
    int iter;
};

auto get_next(file_reader &fr) -> int
{
    return fr.iter = getc(fr.self);
}

void sc_(file_reader &fr)
{
    while (fr.iter != EOF && fr.iter != '\n')
    {
        get_next(fr);
    }
}

void sw_(file_reader &fr, position &pos)
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

// void gt_(std::vector<token> &tokens, file_reader &fr, position &pos)
// {
//     auto str = std::string();

//     if (fr.iter == '_' || isalpha(fr.iter))
//     {
//         // get identifiers
//         while ((isalnum(fr.iter) || fr.iter == '_') && fr.iter != EOF)
//         {
//             str.push_back(fr.iter);
//             get_next(fr);
//         }
//     }
//     else if (isdigit(fr.iter))
//     {
//         auto punct_used = 0;
//         // get number literal
//         while ((isdigit(fr.iter) || fr.iter == '.') && fr.iter != EOF && punct_used <= 1)
//         {
//             if (fr.iter == '.')
//             {
//                 punct_used++;
//             }

//             str.push_back(get_next(fr));
//         }
//     }
//     else
//     {
//         str.push_back(get_next(fr));

//         if (str.front() == '"')
//         {
//             while (get_next(fr) != '"')
//             {
//                 error(fr.iter == EOF, "Unterminated string literal");
//                 str.push_back(fr.iter);
//                 ++pos.col;
//             }

//             str.push_back(fr.iter);
//             tokens.emplace_back(token{str, token_t::LIT_STR, pos});
//             pos.col += str.size();
//             return;
//         }
//         else if (std::string("><=-+&|").find(str.front()) != std::string::npos)
//         {
//             auto backup_self = ftell(fr.self);
//             auto backup_iter = fr.iter;

//             str.push_back(get_next(fr));

//             if (keyword_map.find(str) == keyword_map.end())
//             {
//                 fseek(fr.self, backup_self, SEEK_CUR);
//                 fr.iter = backup_iter;
//                 str.pop_back();
//             }
//         }
//     }

//     auto type = keyword_map.find(str);

//     if (is_digits(str))
//     {
//         tokens.emplace_back(token{str, token_t::LIT_NUM, pos});
//     }
//     else if (type == keyword_map.end())
//     {
//         tokens.emplace_back(token{str, token_t::IDENTIFIER, pos});
//     }
//     else
//     {
//         tokens.emplace_back(token{str, type->second, pos});
//     }

//     pos.col += str.size();
// }

void gt_(std::vector<token> &tokens, file_reader &fr, position &pos)
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
    else
    {
        str.push_back(fr.iter);
        get_next(fr);
    }

    tokens.emplace_back(token{str, token_t::UNDEFINED, pos});
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
            sc_(fr);
        }
        else if (isspace(fr.iter))
        {
            sw_(fr, pos);
        }
        else
        {
            gt_(tokens, fr, pos);
        }
    }

    fclose(fr.self);
    return tokens;
}
