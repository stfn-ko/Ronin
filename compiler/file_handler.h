#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// ===-------------------------------------------=== INCLUDES
#include "error_handler.h"

//  ===-------------------------------------------=== FUNC DEC
//  ***---------------------*** NON-VOID FUNCS
char *readf_2buff(char *_fpath);

//  ===-------------------------------------------=== FUNC DEF
char *readf_2buff(char *_fpath)
{
    if (!_fpath)
    {
        err_ex_p("no input files", _fpath, 0);
    }

    FILE *fp = fopen(_fpath, "rb");

    if (!fp)
    {
        err_ex_p("no such file or directory", _fpath, 0);
    }
    if (!strstr(_fpath, ".ro"))
    {
        err_ex_p("invalid file extension", _fpath, 0);
    }

    // get file size
    fseek(fp, 0, SEEK_END);
    unsigned long fs = ftell(fp);
    rewind(fp);

    char *buff = (char *)malloc(sizeof(char) * (fs + 1));

    if (!buff)
    {
        err_ex_p("couldn't allocate memory for text buffer", FL);
    }
    if (fread(buff, 1, fs, fp) != fs)
    {
        err_ex_p("eof reached while reading file", FL);
    }

    fclose(fp);
    buff[fs] = '\0';

    return buff;
}

#endif