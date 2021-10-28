#include "Rend.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstdarg>

#include "Shader.h"
#include "Utils.h"
#include "Application.h"
#include "Renderer.h"

namespace Pontilus
{
    namespace Graphics
    {
        static const vAttrib vAttribDefault[] =
            {
                {PONT_POS,      PONT_FLOAT, 3},
                {PONT_COLOR,    PONT_FLOAT, 4},
                {PONT_TEXCOORD, PONT_FLOAT, 2},
                {PONT_TEXID,    PONT_FLOAT, 1}
            };

        void initRend(Rend &r, unsigned int numVerts)
        {
            Renderer::addRend(r);

            r.data = malloc(getLayoutLen(r) * numVerts);

            r.layoutCount = sizeof(vAttribDefault) / sizeof(vAttrib);
            r.layout = (vAttrib *) malloc(sizeof(vAttribDefault));
            for (int i = 0; i < r.layoutCount; i++)
            {
                r.layout[i] = vAttribDefault[i];
            }
            //printf("0: %d, 4: %d, 8: %d, Last: %d\n", *(int *)(r.data), *(int *)((char *)r.data + 4), *(int *)((char *)r.data + 8), *(int *)((char *)r.data + getLayoutLen(r) * numVerts - 4));

            r.vertCount = numVerts;
        }

        void initRend(Rend &r, unsigned int numVerts, vAttrib *attribs, unsigned int numAttribs)
        {
            r.data = malloc(getLayoutLen(r) * numVerts);
            
            r.layoutCount = numAttribs;
            r.layout = (vAttrib *) malloc(sizeof(vAttrib) * r.layoutCount);
            for (int i = 0; i < r.layoutCount; i++)
            {
                r.layout[i] = attribs[i];
            }

            r.vertCount = numVerts;
        }

        void initRendByShader(Rend &r, Graphics::Shader &s)
        {
            // Waiting on Shader.h's todo to be completed.
        }

        unsigned int getVPropLen(vPropType p)
        {
            unsigned int len = 0;
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

        unsigned int getLayoutLen(Rend &r)
        {
            unsigned int len = 0;
            for (unsigned int i = 0; i < r.layoutCount; i++)
            {
                len += getVPropLen(r.layout[i].type);
            }

            return len;
        }

        // this should, theoretically, return the pointer to a place in the Rend and the size of that attribute.
        off_len getAttribMetaData(Rend &r, vProp p)
        {
            off_len result = { 0, p };

            int offsetInBytes = 0;
            printf("Is it the same as %d?\n", p);
            
            for (int i = 0; i < r.layoutCount; i++)
            {
                printf("Number of elements in r.layout: %d\n", r.layoutCount);
                printf("A quick test accessing r.data: %2.3f\n", *(float *)r.data);
                if (r.layout[i].prop == p)
                {
                printf("Sanity check: %d?\n", p);
                    if (debugMode())
                    {
                        printf("Rend Layout Property #%d: %d\n", i, r.layout[i].prop);
                    }
                    int attribTypeSize = getVPropLen(r.layout[i].type);
                    
                    result.first = offsetInBytes;
                    result.second = attribTypeSize * r.layout[i].size;
                    printf("Attribute #%d size: %d\n", p, attribTypeSize * r.layout[i].size);
                    return result;
                }
                else
                {
                    offsetInBytes += r.layout[i].size * getVPropLen(r.layout[i].type);
                }
            }

            //return;
            fprintf(stderr, "Rend Layout Size: %ld\n", r.layoutCount);
            /*

            // if you've gotten to this point, you've either specified an illegal
            // vProp or the Rend doesn't have this property. I should implement
            // error handling sometime in the future, but for now, byebye, program!
            */
            fprintf(stderr, "Could not query rend for property %d.\n", p);
            exit(-1);
            //return {0, 0};
        }
    }
}