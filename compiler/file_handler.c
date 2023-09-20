// ===-------------------------------------------=== INCLUDES
#include <stdio.h>
#include "error_handler.c"

#define FL __FILE__,__LINE__

// ===-------------------------------------------=== readf_2buff
char *readf_2buff(char *_fpath)
{
    if(!_fpath) err_ex_p("no input files", FL);

    FILE *fp = fopen(_fpath, "rb");

    if(!fp) err_ex_p("no such file or directory", FL);

    if (!strstr(_fpath, ".ro"))  err_ex_p("invalid file extension", FL);

    // get file size
    fseek(fp, 0, SEEK_END);
    unsigned long fs = ftell(fp);
    rewind(fp);

    char *buff = (char *)malloc(sizeof(char) * (fs + 1));

    if(!buff) err_ex_p("couldn't allocate memory for text buffer", FL);

    if(fread(buff, 1, fs, fp) != fs) err_ex_p("eof reached while reading file", FL);

    fclose(fp);
    buff[fs] = '\0';

    return buff;
}