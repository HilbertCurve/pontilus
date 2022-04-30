#include "graphics/rData.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "core/Application.h"
#include "graphics/Shader.h"
#include "graphics/Renderer.h"
#include "utils/Utils.h"

#define MAX_MESH_INDICES 1 << 14

namespace Pontilus
{
    namespace Renderer
    {
        static const vAttrib vAttribDefault[] =
            {
                {PONT_POS, PONT_FLOAT, 3},
                {PONT_COLOR, PONT_FLOAT, 4},
                {PONT_TEXCOORD, PONT_FLOAT, 2},
                {PONT_TEXID, PONT_FLOAT, 1}
                /*{PONT_OTHER, PONT_FLOAT, 3}*/};

        // IMPORTANT!!! Keep the initialization of fields EXACTLY in this order.
        void initRData(rData &r, unsigned int numVerts, Primitive *p)
        {
            r.layoutCount = sizeof(vAttribDefault) / sizeof(vAttrib);

            r.layout = (vAttrib *)malloc(sizeof(vAttribDefault));
            for (unsigned int i = 0; i < r.layoutCount; i++)
            {
                r.layout[i] = vAttribDefault[i];
            }

            r.data = malloc(getLayoutLen(r) * numVerts);
            r.vertCount = numVerts;

            r.primitive = p;
            if (p == &Primitives::NONE)
            {
                r.indexCount = 0;
                return;
            }
            if (p == &Primitives::MESH)
            {
                r.indexCount = 0;
                // TODO: memory efficiency and resizeable rData
                r.indices = (int *)malloc(MAX_MESH_INDICES);
                return;
            }
            int iPerElement = r.primitive->elementSize;
            r.indexCount = ceil(numVerts / r.primitive->vertexCount) * iPerElement;
            r.indices = (int *)malloc(r.indexCount * sizeof(unsigned int));

            for (int i = 0; i < ceil(numVerts / iPerElement); i++) {
                r.primitive->generateIndices(r.indices, i);
            }
        }

        void initRData(rData &r, unsigned int numVerts, Primitive *p, vAttrib *attribs, unsigned int numAttribs)
        {
            // initialize vertex layout
            r.layoutCount = numAttribs;

            r.layout = (vAttrib *)malloc(sizeof(vAttrib) * r.layoutCount);
            for (unsigned int i = 0; i < r.layoutCount; i++)
            {
                r.layout[i] = attribs[i];
            }

            r.data = malloc(getLayoutLen(r) * numVerts);

            r.vertCount = numVerts;

            // initialize index buffer & fill it up
            r.primitive = p;
            if (p == &Primitives::NONE)
            {
                r.indexCount = 0;
                return;
            }
            if (p == &Primitives::MESH)
            {
                r.indexCount = 0;
                // TODO: memory efficiency and resizeable rData
                r.indices = (int *)malloc(MAX_MESH_INDICES);
                return;
            }
            int iPerElement = r.primitive->elementSize;
            r.indexCount = ceil(numVerts / r.primitive->vertexCount) * iPerElement;
            r.indices = (int *)malloc(r.indexCount * sizeof(unsigned int));

            for (int i = 0; i < ceil(numVerts / iPerElement); i++) {
                r.primitive->generateIndices(r.indices, i);
            }
        }

        void initRDataByShader(rData &r, Renderer::Shader &s)
        {
            // Waiting on Shader.h's todo to be completed.
            __pError("Function `initRDataByShader` not implemented yet.")
        }

        void resizeRData(rData &r, unsigned int newNumVerts)
        {
            // ensure r has been initialized
            // every rData should have a layout
            __pAssert(r.layoutCount == 0, "Attempted to resize uninitialized rData.");

            r.data = realloc(r.data, getLayoutLen(r) * newNumVerts);

            if (r.primitive == &Primitives::NONE) return;
            int iPerElement = r.primitive->elementSize;
            r.indices = (int *) realloc(r.indices, ceil(newNumVerts / iPerElement) * iPerElement * sizeof(unsigned int));

            for (int i = 0; i < ceil(newNumVerts / iPerElement); i++)
            {
                r.primitive->generateIndices(r.indices, i);
            }

            r.vertCount = newNumVerts;
        }

        void clearRData(rData &r)
        {
            free(r.data);
            free(r.indices);

            r.vertCount = 0;
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

        int getLayoutLen(rData &r)
        {
            int len = 0;
            for (unsigned int i = 0; i < r.layoutCount; i++)
            {
                len += getVTypeLen(r.layout[i].type) * r.layout[i].count;
            }

            return len;
        }

        // this should, theoretically, return the pointer to a place in the rData and the size of that attribute.
        off_len getAttribMetaData(rData &r, vProp p)
        {
            off_len result = {0, p};

            int offsetInBytes = 0;

            for (int i = 0; i < r.layoutCount; i++)
            {
                if (r.layout[i].prop == p)
                {

                    int attribTypeSize = getVTypeLen(r.layout[i].type);

                    result.first = offsetInBytes;
                    result.second = attribTypeSize * r.layout[i].count;
                    return result;
                }
                else
                {
                    offsetInBytes += r.layout[i].count * getVTypeLen(r.layout[i].type);
                }
            }

            fprintf(stderr, "rData Layout Size: %d\n", r.layoutCount);
            /*

            // if you've gotten to this point, you've either specified an illegal
            // vProp or the rData doesn't have this property. I should implement
            // error handling sometime in the future, but for now, byebye, program!
            */
            fprintf(stderr, "Could not query rData for property %d.\n", p);
            exit(-1);
        }

        void printRData(rData &r)
        {
            printRData(r, r.vertCount);
        }

        void printRData(rData &r, unsigned int numVerts)
        {
            printf("rData at %p: \033[31mPosition\033[0m, \033[32mColor\033[0m, \033[33mTex Coord\033[0m, \033[34mTex ID\033[0m, \033[35mOther\033[0m\n{\n    ", &r);

            int stride = 0;
            for (int i = 0; i < numVerts; i++)
            {
                for (int j = 0; j < r.layoutCount; j++)
                {
                    // set color of text
                    switch (r.layout[j].prop)
                    {
                        case PONT_POS:
                        {
                            printf("\033[31m");
                        } break;
                        case PONT_COLOR:
                        {
                            printf("\033[32m");
                        } break;
                        case PONT_TEXCOORD:
                        {
                            printf("\033[33m");
                        } break;
                        case PONT_TEXID:
                        {
                            printf("\033[34m");
                        } break;
                        case PONT_OTHER:
                        {
                            printf("\033[35m");
                        } break;
                    }

                    off_len result = getAttribMetaData(r, r.layout[j].prop);
                    switch (r.layout[j].type)
                    {
                    case PONT_SHORT:
                    {
                        for (int k = 0; k < r.layout[j].count; k++)
                        {
                            printf("%3d, ", ((short *)((char *)r.data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    case PONT_INT:
                    {
                        for (int k = 0; k < r.layout[j].count; k++)
                        {
                            printf("%3d, ", ((int *)((char *)r.data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    case PONT_UINT:
                    {
                        for (int k = 0; k < r.layout[j].count; k++)
                        {
                            printf("%3d, ", ((unsigned int *)((char *)r.data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    case PONT_FLOAT:
                    {
                        for (int k = 0; k < r.layout[j].count; k++)
                        {
                            printf("%1.2f, ", ((float *)((char *)r.data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    }
                }
                printf("\n    ");
                stride += getLayoutLen(r);
            }
            printf("\b\b\b\b}\n");
        }
    }
}

