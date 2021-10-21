#include <string.h>
#include <stdio.h>

#include "Application.h"

int main(int argc, char *args[])
{
    if (argc > 0)
    {
        for (int i = 0; i < argc; i++)
        {
            if (strncmp(args[i], "debug", 5) == 1)
            {
                Pontilus::args |= 0x0001;
                printf("hello\n");
            }
            printf("%s\n", args[i]);
        }
    }

    Pontilus::init();
    Pontilus::loop();

    return 0;
}
