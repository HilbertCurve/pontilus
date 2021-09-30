#include "Obj3D.h"

#include <GLES3/gl3.h>
#include <stdio.h>

namespace Pontilus
{
    namespace Model
    {
        GLfloat *loadObjFromBinary(const char *filepath, Obj3D &dest)
        {
            FILE *ptr = fopen(filepath, "rb");
            long filesize = 0;

            if (ptr == nullptr)
            {
                fprintf(stderr, "Could not open %s.\n", filepath);
                return nullptr;
            }

            fseek(ptr, 0L, SEEK_END);
            filesize = ftell(ptr);
            rewind(ptr);

            dest.numFloats = filesize/4;

            dest.vertexBufferData = new GLfloat[filesize/4];

            fread(dest.vertexBufferData, filesize/4, 4, ptr);
            
            for (int i = 0; i < filesize/4; i++)
            {
                printf("%1.2f\n", dest.vertexBufferData[i]);
            }

            printf("\b\b\n");

            printf("Filesize in floats: %ld\n", dest.numFloats);

            return dest.vertexBufferData;
        }
    }
}