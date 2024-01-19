#include "renderer/rData.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "core/Application.h"
#include "renderer/Shader.h"
#include "renderer/Renderer.h"
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

        rData::rData() {
            *this = rData(0, &Primitives::NONE);
        }

        rData::rData(unsigned int numVerts, Primitive *p)
        {
            this->dataOffset = 0;

            this->layoutCount = sizeof(vAttribDefault) / sizeof(vAttrib);

            this->layout = (vAttrib *)malloc(sizeof(vAttribDefault));
            for (unsigned int i = 0; i < this->layoutCount; i++)
            {
                this->layout[i] = vAttribDefault[i];
            }

            this->data = malloc(this->getLayoutLen() * numVerts);
            this->vertCount = numVerts;

            this->primitive = p;
            if (p == &Primitives::NONE)
            {
                this->indexCount = 0;
                return;
            }
            if (p == &Primitives::MESH)
            {
                this->indexCount = 0;
                // TODO: memory efficiency and resizeable rData
                this->indices = (int *)malloc(MAX_MESH_INDICES);
                return;
            }
            int iPerElement = this->primitive->elementSize;
            this->indexCount = ceil(numVerts / this->primitive->vertexCount) * iPerElement;
            this->indices = (int *)malloc(this->indexCount * sizeof(unsigned int));

            for (int i = 0; i < ceil(numVerts / iPerElement); i++) {
                this->primitive->generateIndices(this->indices, i);
            }
        }

        rData::rData(unsigned int numVerts, Primitive *p, vAttrib *attribs, unsigned int numAttribs)
        {
            this->dataOffset = 0;
            // initialize vertex layout
            this->layoutCount = numAttribs;

            this->layout = (vAttrib *)malloc(sizeof(vAttrib) * this->layoutCount);
            for (unsigned int i = 0; i < this->layoutCount; i++)
            {
                this->layout[i] = attribs[i];
            }

            this->data = malloc(this->getLayoutLen() * numVerts);

            this->vertCount = numVerts;

            // initialize index buffer & fill it up
            this->primitive = p;
            if (p == &Primitives::NONE)
            {
                this->indexCount = 0;
                return;
            }
            if (p == &Primitives::MESH)
            {
                this->indexCount = 0;
                // TODO: memory efficiency and resizeable rData
                this->indices = (int *)malloc(MAX_MESH_INDICES);
                return;
            }
            int iPerElement = this->primitive->elementSize;
            this->indexCount = ceil(numVerts / this->primitive->vertexCount) * iPerElement;
            this->indices = (int *)malloc(this->indexCount * sizeof(unsigned int));

            for (int i = 0; i < ceil(numVerts / iPerElement); i++) {
                this->primitive->generateIndices(this->indices, i);
            }
        }

        void rData::resize(unsigned int newNumVerts)
        {
            // ensure r has been initialized
            // every rData should have a layout
            __pAssert(this->layoutCount == 0, "Attempted to resize uninitialized rData.");

            this->data = realloc(this->data, this->getLayoutLen() * newNumVerts);

            if (this->primitive == &Primitives::NONE) return;
            int iPerElement = this->primitive->elementSize;
            this->indices = (int *) realloc(this->indices, ceil(newNumVerts / iPerElement) * iPerElement * sizeof(unsigned int));

            for (int i = 0; i < ceil(newNumVerts / iPerElement); i++)
            {
                this->primitive->generateIndices(this->indices, i);
            }

            this->vertCount = newNumVerts;
        }

        void rData::clear()
        {
            free(this->data);
            free(this->indices);

            this->vertCount = 0;
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

        int rData::getLayoutLen()
        {
            int len = 0;
            for (unsigned int i = 0; i < this->layoutCount; i++)
            {
                len += getVTypeLen(this->layout[i].type) * this->layout[i].count;
            }

            return len;
        }

        // this should, theoretically, return the pointer to a place in the rData and the size of that attribute.
        rData::off_len rData::getAttribMetaData(vProp p)
        {
            off_len result = {0, p};

            int offsetInBytes = 0;

            for (int i = 0; i < this->layoutCount; i++)
            {
                if (this->layout[i].prop == p)
                {

                    int attribTypeSize = getVTypeLen(this->layout[i].type);

                    result.first = offsetInBytes;
                    result.second = attribTypeSize * this->layout[i].count;
                    return result;
                }
                else
                {
                    offsetInBytes += this->layout[i].count * getVTypeLen(this->layout[i].type);
                }
            }

            fprintf(stderr, "rData Layout Size: %d\n", this->layoutCount);
            /*

            // if you've gotten to this point, you've either specified an illegal
            // vProp or the rData doesn't have this property. I should implement
            // error handling sometime in the future, but for now, byebye, program!
            */
            fprintf(stderr, "Could not query rData for property %d.\n", p);
            exit(-1);
        }

        void rData::print()
        {
            this->print(this->vertCount);
        }

        void rData::print(unsigned int numVerts)
        {
            printf("rData at %p: \033[31mPosition\033[0m, \033[32mColor\033[0m, \033[33mTex Coord\033[0m, \033[34mTex ID\033[0m, \033[35mOther\033[0m\n{\n    ", this);

            int stride = 0;
            for (int i = 0; i < numVerts; i++)
            {
                for (int j = 0; j < this->layoutCount; j++)
                {
                    // set color of text
                    switch (this->layout[j].prop)
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

                    off_len result = rData::getAttribMetaData(this->layout[j].prop);
                    switch (this->layout[j].type)
                    {
                    case PONT_SHORT:
                    {
                        for (int k = 0; k < this->layout[j].count; k++)
                        {
                            printf("%3d, ", ((short *)((char *)this->data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    case PONT_INT:
                    {
                        for (int k = 0; k < this->layout[j].count; k++)
                        {
                            printf("%3d, ", ((int *)((char *)this->data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    case PONT_UINT:
                    {
                        for (int k = 0; k < this->layout[j].count; k++)
                        {
                            printf("%3d, ", ((unsigned int *)((char *)this->data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    case PONT_FLOAT:
                    {
                        for (int k = 0; k < this->layout[j].count; k++)
                        {
                            printf("%1.2f, ", ((float *)((char *)this->data + result.first + stride))[k]);
                        }
                        printf("\033[0m");
                    }
                    break;
                    }
                }
                printf("\n    ");
                stride += this->getLayoutLen();
            }
            printf("\b\b\b\b}\n");
        }
    }
}

