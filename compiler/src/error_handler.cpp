#include "../inc/error_handler.h"

void report(bool condition, std::string error_message, bool critical)
{
    if (condition)
    {
        std::cerr << error_message << std::endl;
        if (critical)
            exit(EXIT_FAILURE);
    }
}

