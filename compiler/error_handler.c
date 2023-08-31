// ===-------------------------------------------=== INCLUDES
#include <stdlib.h>
#include <string.h>
#include "ANSI.h"

// ===-------------------------------------------=== print error on exit
void perroex(char *_err_msg)
{
    printf(UL_Red "\n\terror -->" Reset " %s\n\n", _err_msg);
    exit(EXIT_FAILURE);
}