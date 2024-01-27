#include "inc/parser.h"

int main(int argc, char *argv[])
{
    parse(scan(argv[1]));
    return 0;
}