// ===-------------------------------------------=== INCLUDES
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ANSI.h"

// ===-------------------------------------------=== print error on exit
void err_ex_p(const char *_err_msg, const char *__FILE, uint32_t __LINE)
{
    if (__LINE)
    {
        printf(UL_Red "\nERROR" Reset B_Red " ( %s | ln:%d ) >>> " Reset "%s\n\n", __FILE, __LINE, _err_msg);
    }
    else
    {
        printf(UL_Red "\nERROR" Reset B_Red " ( %s ) >>> " Reset "%s\n\n", __FILE, _err_msg);
    }

    exit(EXIT_FAILURE);
}

// ===-------------------------------------------=== DEBUG
void todo_err(const char *_msg, const char *__FILE, uint32_t __LINE)
{
    printf(UL_Purple "\nTODO" Reset B_Purple " ( %s | ln:%d ) >>> " Reset "%s\n\n",
           __FILE, __LINE, _msg);

    exit(EXIT_FAILURE);
}