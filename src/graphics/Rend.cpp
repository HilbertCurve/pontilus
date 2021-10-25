#include "Rend.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstdarg>

#include "Shader.h"
#include "Utils.h"
#include "Application.h"

namespace Pontilus
{
    namespace Graphics
    {
        static vAttrib vAttribDefault[] =
            {
                {PONT_POS, PONT_FLOAT, 3},
                {PONT_COLOR, PONT_FLOAT, 4},
                {PONT_TEXCOORD, PONT_FLOAT, 2},
                {PONT_TEXID, PONT_FLOAT, 1}};

        void initRend(Rend &r, unsigned int numVerts)
        {
            r.layout.assign(vAttribDefault, vAttribDefault + 4);

            r.data = malloc(getLayoutLen(r) * numVerts);

            r.vertCount = numVerts;
        }

        void initRend(Rend &r, unsigned int numVerts, vAttrib *attribs, unsigned int numAttribs)
        {
            r.layout.assign(attribs, attribs + numAttribs);

            r.data = malloc(getLayoutLen(r) * numVerts);

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
            for (unsigned int i = 0; i < r.layout.size(); i++)
            {
                len += getVPropLen(r.layout[i].type);
            }

            return len;
        }

        // this should, theoretically, return the pointer to a place in the Rend and the size of that attribute.
        pos_len getAttribMetaData(Rend &r, vProp p)
        {
            pos_len result = { nullptr, 0 };

            char *posInBytes = (char *)&r;
            
            for (int i = 0; i < r.layout.size(); i++)
            {
                if (r.layout[i].prop == p)
                {
                    if (debugMode())
                    {
                        printf("Rend Layout Property #%d: %d\n", i, r.layout[i].prop);
                    }
                    int attribTypeSize = getVPropLen(r.layout[i].type);
                     
                    printf("beans\n");
                    //result.first = (void *)&r;
                    result.first = malloc(10);
                    result.second = 100;
                    printf("well that worked \n");
                    return result;
                }
                else
                {
                    //posInBytes += r.layout[i].size * getVPropLen(r.layout[i].type);
                }
            }

            fprintf(stderr, "Rend Layout Size: %ld", r.layout.size());

            // if you've gotten to this point, you've either specified an illegal
            // vProp or the Rend doesn't have this property. I should implement
            // error handling sometime in the future, but for now, byebye, program!
            fprintf(stderr, "Could not query rend for property %d.\n", p);
            exit(-1);
            return {0, 0};
        }
    }
}