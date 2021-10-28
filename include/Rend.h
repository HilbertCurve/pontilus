#pragma once

#include <vector>

#include "Shader.h"
#include "Utils.h"

namespace Pontilus
{
    namespace Graphics
    {
        enum vProp
        {
            PONT_POS,
            PONT_COLOR,
            PONT_TEXCOORD,
            PONT_TEXID,
            PONT_OTHER
        };
        enum vPropType
        {
            PONT_SHORT,
            PONT_INT,
            PONT_UINT,
            PONT_FLOAT
        };

        struct vAttrib
        {
            vProp prop;
            vPropType type;
            unsigned int size;
        };

        /**
         * A Rend (short for Renderable Element-based Nugget of Data) is an array
         * with properties that align with a specific shader. They are intended
         * to be pieces of data with a sensible structure that allows for 
         * easy access and pipelining.
         */
        struct Rend
        {
            void *data;
            
            vAttrib *layout;
            unsigned int layoutCount;
            unsigned int vertCount;
        };

        void initRend(Rend &r, unsigned int numVerts);
        void initRend(Rend &r, unsigned int numVerts, vAttrib *attribs, unsigned int numAttribs); // I'd love to know how to do typed variadic arguments without templates, but I guess not right now 
        void initRendByShader(Rend &r, Graphics::Shader &s);

        int getVTypeLen(vProp p);
        int getLayoutLen(Rend &r);

        typedef BiReturn off_len;
        extern off_len result;
        off_len getAttribMetaData(Rend &r, vProp p);

        /**
         * Gets the pointer to the first instance of Rend r's vertex attribute property p.
         */
        void *getAttribPointer(Rend &r, vProp p);
    }
}