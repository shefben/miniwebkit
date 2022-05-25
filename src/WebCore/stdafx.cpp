#include "stdafx.h"

extern "C" FILE * __iob_func()
{
    static FILE files[] = { *(stdin), *(stdout), *(stderr) };
    return files;
}
