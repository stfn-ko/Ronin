#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <unordered_map>

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

    KW_ISIZE,
    KW_I32,
    KW_USIZE,
    KW_U32,
    KW_F32,
    KW_STR,

    KW_IF,
    KW_ELSE,

    KW_FOR,
    KW_WHILE,

    KW_RETURN,

    //

    COMB_GTOE,
    COMB_LTOE,
    COMB_EQ,
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

typedef struct
{
    size_t ln;
    size_t col;
} position;

typedef struct
{
    std::string lexeme;
    token_t type;
    position pos;
} token;

typedef struct
{

} AST;

void report(bool condition, std::string error_message, bool critical = true)
{
    if (condition)
    {
        std::cerr << error_message << std::endl;
        if (critical)
            exit(EXIT_FAILURE);
    }
}

auto valid_extension(const std::string &file_name) -> bool
{
    size_t dpos = file_name.find_last_of('.');
    if (dpos != file_name.npos)
    {
        std::string ext(file_name, dpos + 1);
        return ext == "ro";
    }

    return false;
}

auto read_file(const std::string &file_path) -> std::unique_ptr<std::string>
{
    report(file_path.empty(), "No input file specified");
    report(!valid_extension(file_path), "Invalid file extension type");

    std::ifstream ifs(file_path);

    report(!ifs.is_open(), "Failed to open file: " + file_path);

    return std::make_unique<std::string>(std::istreambuf_iterator<char>(ifs),
                                         std::istreambuf_iterator<char>());
}

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

void get_tokens(auto &tokens, auto &it, auto &pos, auto end)
{
    auto start = it;

    while (!is_delim(it) && it != end)
    {
        ++it;
    }

    if (start == it)
    {
        ++it;
    }

    if (*start == '"')
    {
        while (*(++it) != '"')
        {
            report(it == end, "Syntax Error: Unterminated string literal");
        }

        tokens.emplace_back(token{std::string(start, ++it), token_t::LIT_STR, pos});
        pos.col += it - start;
        return;
    }
    else if (std::string("+-=></").find(*start) != std::string::npos)
    {
        // /rwsd
        // ^
        //     ^
        int iter = 4;
        auto tmp_str = std::string(start, it + iter);
        std::cout<< tmp_str << "|\n";

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

auto tokenize(const std::string &path) -> std::vector<token>
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

auto parse(std::vector<token> tokens) -> AST
{
    for (const auto &t : tokens)
    {
        std::cout
            << "type: "
            << t.type << " | (ln:"
            << t.pos.ln << ", | col:"
            << t.pos.col << ")\n"
            << t.lexeme << "\n\n";
    }

    return AST();
}

int main(int argc, char *argv[])
{
    parse(tokenize(argv[1]));
    return 0;
}