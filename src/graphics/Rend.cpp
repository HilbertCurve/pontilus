#include "Rend.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstdarg>

#include "Shader.h"
#include "Utils.h"
#include "Application.h"
#include "Renderer.h"

// TODO: change the name Rend, it sucks.

namespace Pontilus
{
    namespace Graphics
    {
        static const vAttrib vAttribDefault[] =
            {
                {PONT_POS, PONT_FLOAT, 3},
                {PONT_COLOR, PONT_FLOAT, 4},
                {PONT_TEXCOORD, PONT_FLOAT, 2},
                {PONT_TEXID, PONT_FLOAT, 1}};

        // IMPORTANT!!! Keep the initialization of fields EXACTLY in this order.
        void initRend(Rend &r, unsigned int numVerts)
        {
            Renderer::addRend(r);

            r.layoutCount = sizeof(vAttribDefault) / sizeof(vAttrib);

            r.layout = (vAttrib *)malloc(sizeof(vAttribDefault));
            for (int i = 0; i < r.layoutCount; i++)
            {
                r.layout[i] = vAttribDefault[i];
            }

            r.data = malloc(getLayoutLen(r) * numVerts);

            r.vertCount = numVerts;
        }

        void initRend(Rend &r, unsigned int numVerts, vAttrib *attribs, unsigned int numAttribs)
        {
            Renderer::addRend(r);

            r.layoutCount = numAttribs;

            r.layout = (vAttrib *)malloc(sizeof(vAttrib) * r.layoutCount);
            for (int i = 0; i < r.layoutCount; i++)
            {
                r.layout[i] = attribs[i];
            }

            r.data = malloc(getLayoutLen(r) * numVerts);

            r.vertCount = numVerts;
        }

        void initRendByShader(Rend &r, Graphics::Shader &s)
        {
            // Waiting on Shader.h's todo to be completed.
        }

        void resizeRend(Rend &r, unsigned int newNumVerts)
        {
            // ensure r has been initialized
            if (r.layoutCount == 0) // every rend should have a layout
            {
                initRend(r, newNumVerts);
                return;
            }

            r.data = realloc(r.data, getLayoutLen(r) * newNumVerts);
        }

        int getVTypeLen(vPropType p)
        {
            int len = 0;
            switch (p)
            {
            case PONT_SHORT:
                len = sizeof(short);
                break;
            case PONT_INT:
                len = sizeof(int);
                break;
            case PONT_UINT:
                len = sizeof(unsigned int);
                break;
            case PONT_FLOAT:
                len = sizeof(float);
                break;
            default:
                break; // error handling'll come later
            }

            return len;
        }

        int getLayoutLen(Rend &r)
        {
            int len = 0;
            for (int i = 0; i < r.layoutCount; i++)
            {
                len += getVTypeLen(r.layout[i].type) * r.layout[i].size;
            }

            return len;
        }

        // this should, theoretically, return the pointer to a place in the Rend and the size of that attribute.
        off_len getAttribMetaData(Rend &r, vProp p)
        {
            off_len result = {0, p};

            int offsetInBytes = 0;

            for (int i = 0; i < r.layoutCount; i++)
            {
                if (r.layout[i].prop == p)
                {

                    int attribTypeSize = getVTypeLen(r.layout[i].type);

                    result.first = offsetInBytes;
                    result.second = attribTypeSize * r.layout[i].size;
                    return result;
                }
                else
                {
                    offsetInBytes += r.layout[i].size * getVTypeLen(r.layout[i].type);
                }
            }

            fprintf(stderr, "Rend Layout Size: %ld\n", r.layoutCount);
            /*

            // if you've gotten to this point, you've either specified an illegal
            // vProp or the Rend doesn't have this property. I should implement
            // error handling sometime in the future, but for now, byebye, program!
            */
            fprintf(stderr, "Could not query rend for property %d.\n", p);
            exit(-1);
        }

        void printRend(Rend &r)
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
                    }
                    break;
                    case Graphics::PONT_INT:
                    {
                        for (int k = 0; k < r.layout[j].size; k++)
                        {
                            printf("%3d, ", ((int *)((char *)r.data + result.first + stride))[k]);
                        }
                    }
                    break;
                    case Graphics::PONT_UINT:
                    {
                        for (int k = 0; k < r.layout[j].size; k++)
                        {
                            printf("%3d, ", ((unsigned int *)((char *)r.data + result.first + stride))[k]);
                        }
                    }
                    break;
                    case Graphics::PONT_FLOAT:
                    {
                        for (int k = 0; k < r.layout[j].size; k++)
                        {
                            printf("%1.2f, ", ((float *)((char *)r.data + result.first + stride))[k]);
                        }
                    }
                    break;
                    }
                }
                printf("\n    ");
                stride += Graphics::getLayoutLen(r);
            }
            printf("\b\b\b\b}\n");
        }
    }
}