#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ANSI.h"

#define FL __FILE__, __LINE__

//  ===-------------------------------------------=== FUNC DEC
//  ***---------------------*** VOID FUNCS
void error_exit(const char *_err_msg, const char *__FILE, size_t __LINE);
void todo_message(const char *_msg, const char *__FILE, size_t __LINE);

#endif