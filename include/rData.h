//#pragma once

#ifndef _PONTILUS_REND_H
#define _PONTILUS_REND_H

#include <vector>

#include <GL/gl.h>

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
            unsigned int count;
        };

        /**
         * A Rend (short for Renderable Element-based Nugget of Data) is an array
         * with properties that align with a specific shader. They are intended
         * to be pieces of data with a sensible structure that allows for 
         * easy access and pipelining.
         */
        struct rData
        {
            void *data;
            
            vAttrib *layout;
            unsigned int layoutCount;
            unsigned int vertCount;

            bool isDirty = false;
        };

        void initRData(rData &r, unsigned int numVerts);
        void initRData(rData &r, unsigned int numVerts, vAttrib *attribs, unsigned int numAttribs); // I'd love to know how to do typed variadic arguments without templates, but I guess not right now 
        void initRDataByShader(rData &r, Graphics::Shader &s);

        void resizeRData(rData &r, unsigned int newNumVerts);

        int getVTypeLen(vPropType p);
        int getLayoutLen(rData &r);

        void printRData(rData &r);
        void printRData(rData &r, unsigned int numVerts);

        typedef Pair<int, int> off_len;
        off_len getAttribMetaData(rData &r, vProp p);

        /**
         * Gets the pointer to the first instance of Rend r's vertex attribute property p.
         */
        void *getAttribPointer(rData &r, vProp p);
    }
}

#endif // _PONTILUS_REND_H
