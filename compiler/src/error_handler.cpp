#include "../inc/error_handler.h"

auto mkerr(std::string &context, std::string &message, errc error_code, position position) -> error_t
{
    return error_t{.context = context, .msg = message, .code = error_code, .pos = position};
}

void error(bool condition, std::string msg)
{
    if (condition)
    {
        std::cout 
            << BOLD_RED "\n    ERROR >> " COLOR_RESET 
            << msg 
            << "\n\n";

        exit(EXIT_FAILURE);
    }
}

void warning(bool condition, error_t err)
{
    if (condition)
    {
        std::cout << err.msg << "\n";
    }
}

void report(bool condition, error_t err)
{
    if (condition)
    {
        std::string col_str(err.pos.col, ' ');

        std::cout << "\nERROR: "
                  << err.code
                  << " >> "
                  << err.msg
                  << "\n    "
                  << err.pos.ln
                  << " | "
                  << err.context
                  << "\n";

        std::cout << "                                     "
                  << col_str
                  << "^~~ here\n";

        exit(EXIT_FAILURE);
    }
}