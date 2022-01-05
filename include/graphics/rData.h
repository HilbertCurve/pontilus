//#pragma once

#pragma once

#include <vector>

#include <GL/gl.h>

#include "graphics/Shader.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace Graphics
    {
        // classes that implement this interface must 
        // specify how they put vertex data into an rData
        class Renderable
        {
            virtual int toRData(Graphics::rData &r, unsigned int rOffset) = 0;
            virtual void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property) = 0;
        };

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
         * An rData instance is a packet-like set of metadata describing a region of
         * memory that will be sent to the GPU with properties that align with an 
         * attached shader.
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
         * Gets the pointer to the first instance of r's vertex attribute property p.
         */
        void *getAttribPointer(rData &r, vProp p);
    }
}
