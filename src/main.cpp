#include <string.h>

#include "Application.h"

int main(int argc, char *args[])
{
    if (argc > 0)
    {
        Pontilus::_PONTILUS_SETTINGS *a = Pontilus::getArgs();
        for (int i = 0; i < argc; i++)
        {
            if (strcmp(args[i], "debug") == 0)
            {
                *a |= 0x0001;
            }
        }
    }

    Pontilus::init();
    Pontilus::loop();

    return 0;
}
