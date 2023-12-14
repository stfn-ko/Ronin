#include "../inc/file_handler.h"

/***********************************************************/
/*                  FUNCTION DEFINITIONS                   */
/***********************************************************/

char *readf_2buff(char *_fpath)
{
    if (!_fpath)
    {
        error_exit("no input files", _fpath, 0);
    }

    FILE *fp = fopen(_fpath, "rb");

    if (!fp)
    {
        error_exit("no such file or directory", _fpath, 0);
    }
    if (!strstr(_fpath, ".ro"))
    {
        error_exit("invalid file extension", _fpath, 0);
    }

    // get file size
    fseek(fp, 0, SEEK_END);
    unsigned long fs = ftell(fp);
    rewind(fp);

    char *buff = (char *)malloc(sizeof(char) * (fs + 1));

    if (!buff)
    {
        error_exit("couldn't allocate memory for text buffer", FL);
    }
    if (fread(buff, 1, fs, fp) != fs)
    {
        error_exit("eof reached while reading file", FL);
    }

    fclose(fp);
    buff[fs] = '\0';

    return buff;
}