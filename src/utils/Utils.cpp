#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "Rend.h"
#include "GameObject.h"
#include "Renderer.h"

namespace Pontilus
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

    using namespace Graphics;
    void printRend(Graphics::Rend &r)
    {
        printf("Rend at %p:\n{\n    ", &r);

        int stride = 0;
        for (int i = 0; i < r.vertCount; i++)
        {
            for (int j = 0; j < r.layoutCount; j++)
            {
                Graphics::off_len result = Graphics::getAttribMetaData(r, r.layout[j].prop);
                switch (r.layout[j].type)
                {
                    case Graphics::PONT_SHORT: 
                    {
                        for (int k = 0; k < r.layout[j].size; k++)
                        {
                            printf("%3s, ", ((short *)((char *)r.data + result.first + stride))[k]);
                        }
                    } break;
                    case Graphics::PONT_INT: 
                    {
                        for (int k = 0; k < r.layout[j].size; k++)
                        {
                            printf("%3d, ", ((int *)((char *)r.data + result.first + stride))[k]);
                        }
                    } break;
                    case Graphics::PONT_UINT: 
                    {
                        for (int k = 0; k < r.layout[j].size; k++)
                        {
                            printf("%3d, ", ((unsigned int *)((char *)r.data + result.first + stride))[k]);
                        }
                    } break;
                    case Graphics::PONT_FLOAT: 
                    {
                        for (int k = 0; k < r.layout[j].size; k++)
                        {
                            printf("%1.2f, ", ((float *)((char *)r.data + result.first + stride))[k]);
                        }
                    } break;
                }
            }
            printf("\n    ");
            stride += Graphics::getLayoutLen(r);
        }
        printf("\b\b\b\b}\n");
    }
}