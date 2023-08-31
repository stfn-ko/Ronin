// ===-------------------------------------------=== INCLUDES
#include <stdio.h>
#include "error_handler.c"

// ===-------------------------------------------=== readf_2buff
char *readf_2buff(char *_fpath)
{
    if(!_fpath) perroex("no input files");

    FILE *fp = fopen(_fpath, "rb");

    if(!fp) perroex("no such file or directory");

    if (!strstr(_fpath, ".ro"))  perroex("invalid file extension");

    // get file size
    fseek(fp, 0, SEEK_END);
    unsigned long fs = ftell(fp);
    rewind(fp);

    char *buff = (char *)malloc(sizeof(char) * (fs + 1));

    if(!buff) perroex("out of memory");

    if(fread(buff, 1, fs, fp) != fs) perroex("eof reached while reading file");

    fclose(fp);
    buff[fs] = '\0';

    return buff;
}