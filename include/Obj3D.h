/* date = September 28th 2021 9:27 am */

#ifndef _PONTILUS_OBJ_3D_H
#define _PONTILUS_OBJ_3D_H

#include <string>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace Pontilus
{
    namespace Model
    {
        struct GLTF
        {
            struct Accessor
            {
                short bufferView;
                short componentType;
                unsigned int count;
                int *max;
                int *min;
                const char *type;
            };
            Accessor *m_accessors;

            struct Asset
            {
                const char *copyright;
                const char *generator;
                const char *version;
            };

            struct BufferView
            {
                int buffer;
                unsigned int byteLength;
                unsigned int byteOffset;
                unsigned int target;
            };
            BufferView *m_bufferViews;

            struct Buffer
            {
                unsigned int byteLength;
                const char *uri;
            };
            Buffer *m_buffers;

            const char *m_Images;

            struct Material
            {
                const char *name;
                const char *alphaMode;
                int baseColorFactor[4];
                unsigned int baseColorTexture;
                unsigned int normalTexture;
                float metallicFactor;
            };
            Material *m_materials;

            struct Mesh
            {
                const char *name;
                union
                {
                    unsigned int *attributes_indices;
                    unsigned int indices;
                    unsigned int material;
                } primitives;
            };
            Mesh *m_meshes;

            struct Nodes
            {
                int mesh;
                const char *name;
                glm::vec3 translation;
            };
            Nodes *m_nodes;

            struct Scenes
            {
                unsigned int index;
                const char *name;
                unsigned int *nodes;
            };
            Scenes *m_scenes;

            unsigned int *samplers;
            struct Textures
            {
                unsigned int sampler;
                unsigned int source;
            };
        };

        struct Obj3D
        {
            const char *filepath;
            long numFloats;

            GLfloat *vertexBufferData;
        };

        GLfloat *loadObjFromBinary(const char *filepath, Obj3D &dest);
    }
}

#endif