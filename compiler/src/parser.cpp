#include "../inc/parser.h"

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
