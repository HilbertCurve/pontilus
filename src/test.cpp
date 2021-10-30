#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"

#define EXIT asm ("hlt")

int main()
{
    __pError("oh no");

    return 0;
}