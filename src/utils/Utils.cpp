#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>

namespace Pontilus
{
    namespace Utils
    {
        void loadFile(const char *filepath, char *data)
        {
            FILE *ptr = fopen(filepath, "rb");
            long filesize = 0;

            if (ptr == nullptr)
            {
                fprintf(stderr, "Could not open \"%s\".\n", filepath);
                exit(-1);
            }

            fseek(ptr, 0L, SEEK_END);
            filesize = ftell(ptr);
            rewind(ptr);

            data = new char[filesize];

            fread((void *)data, filesize, 1, ptr);
        }
    }
}