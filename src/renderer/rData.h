#pragma once

#include <vector>

#include <glad/gl.h>

#include "renderer/Primitive.h"
#include "renderer/Shader.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace Renderer
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
         * An rData instance is a packet-like set of metadata describing a region of
         * memory that will be sent to the GPU with properties that align with an 
         * attached shader.
         */
        struct rData
        {
            rData();
            rData(unsigned int numVerts, Primitive *p);
            rData(unsigned int numVerts, Primitive *p, vAttrib *attribs, unsigned int numAttribs);
            ~rData() { this->clear(); }

            void clear();

            void resize(unsigned int newNumVerts);

            void print();
            void print(unsigned int numVerts);

            int getLayoutLen();
            typedef Pair<size_t, size_t> off_len;
            off_len getAttribMetaData(vProp p);

            /**
             * Gets the pointer to the first instance of r's vertex attribute property p.
             */
            void *getAttribPointer(rData &r, vProp p);

            // vertex data
            void *data;
            vAttrib *layout;
            unsigned int layoutCount;
            unsigned int vertCount;
            // the offset in bytes one must travel to reach unused memory
            unsigned int dataOffset = 0;

            // index data
            Primitive *primitive;
            int *indices = nullptr;
            unsigned int indexCount = 0;

            // OpenGL handles
            GLuint vao, vbo, ebo;

            bool isDirty = false;
        };

        // classes that implement this interface must 
        // specify how they put vertex data into an rData
        class Renderable
        {
            virtual int toRData(Renderer::rData &r) = 0;
        };

        int getVTypeLen(vPropType p);
    }
}

