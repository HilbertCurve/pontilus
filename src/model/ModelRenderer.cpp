#include "model/ModelRenderer.h"

#include "graphics/Renderer.h"
#include "ecs/Component.h"
#include <glm/gtx/transform.hpp>
#include <nlohmann/json.hpp>
#include <string> // std::string::rfind, ::substr, ::c_str, copy constructor
#include <stdlib.h> // random

namespace Pontilus
{
    namespace Model
    {
        /**
         * Initializes a ModelRenderer component, filling up its cache.
         */
        void ModelRenderer::init(const char *gltfFile)
        {
            this->gltfData = getJSON(gltfFile);
            gltfFP = gltfFile;

            // get relative filepath
            std::string fp = std::string(gltfFile);
            size_t lastSlash = fp.rfind('/');
            fp = fp.substr(0, lastSlash + 1);

            // get binary name
            std::string bin = gltfData["buffers"][0]["uri"].get<std::string>();
            fp += bin;

            binFP = fp;

            // get buffer info
            int posAccessor, /*colorAccessor,*/ normalAccessor, indexAccessor;
            auto gltfPrim = gltfData["meshes"][0]["primitives"][0];

            posAccessor = gltfPrim["attributes"]["POSITION"];
            // colorAccessor = gltfPrim["attributes"]["COLOR"];
            normalAccessor = gltfPrim["attributes"]["POSITION"];
            indexAccessor = gltfPrim["indices"];

            // for each accessor, get info about buffer layout
            auto accessors = gltfData["accessors"];
            int posLoc = accessors[posAccessor]["bufferView"];
            int normalLoc = accessors[normalAccessor]["bufferView"];
            int indexLoc = accessors[indexAccessor]["bufferView"];

            // get data types
            posType = accessors[posAccessor]["componentType"];
            normalType = accessors[normalAccessor]["componentType"];
            indexType = accessors[indexAccessor]["componentType"];

            posCount = accessors[posAccessor]["count"];
            normalCount = accessors[normalAccessor]["count"];
            indexCount = accessors[indexAccessor]["count"];

            // get buffer lengths
            auto bufferViews = gltfData["bufferViews"];

            posLength = bufferViews[posLoc]["byteLength"];
            normalLength = bufferViews[normalLoc]["byteLength"];
            indexLength = bufferViews[indexLoc]["byteLength"];

            posOffset = bufferViews[posLoc]["byteOffset"];
            normalOffset = bufferViews[normalLoc]["byteOffset"];
            indexOffset = bufferViews[indexLoc]["byteOffset"];
        }
        
        static bool warned1 = false;
        static bool warned2 = false;
        int ModelRenderer::toRData(Renderer::rData &r, unsigned int rOffset)
        {
            using namespace Renderer;
            if (rOffset && !warned1)
            {
                __pWarning("rOffset unused in model rendering due to complex index buffers.");
                warned1 = true;
            }

            FILE *f = fopen(binFP.c_str(), "rb");
            if (!f)
            {
                if (!warned2)
                {
                    __pWarning("File %s could not be opened.", binFP.c_str());
                    warned2 = true;
                }
                return 0;
            }

            // fill up pos buffer
            fseek(f, posOffset, SEEK_SET);
            auto offlen = getAttribMetaData(r, PONT_POS);
            auto colorOfflen = getAttribMetaData(r, PONT_COLOR);
            int chunkSize = (int) (posLength / posCount);
            int vertSize = getLayoutLen(r);
            // TODO: rotation and translation can be cached with GameObject?
            glm::mat4 rotation = glm::rotate(parent->rotation.x, glm::vec3{1.0f, 0.0f, 0.0f}) *
                glm::rotate(parent->rotation.y, glm::vec3{0.0f, 1.0f, 0.0f}) *
                glm::rotate(parent->rotation.z, glm::vec3{0.0f, 0.0f, 1.0f});
            // glm::mat4 translation = glm::translate(parent->pos);

            int numZero = 0;
            for (int i = 0; i < posCount; i++)
            {
                float vertex[3];

                fread(&vertex[0], sizeof(float), 3, f);

                glm::vec3 glmVec = {vertex[0], vertex[1], vertex[2]};

                static const float SCALE = 5.0f;

                glmVec = rotation * glm::vec4(glmVec, 1.0f);
                glmVec *= SCALE;
                glmVec += parent->pos;

                memcpy(&((char *) r.data)[offlen.first + i * vertSize], (float *)&glmVec, sizeof(float) * 3);

                const glm::vec4 COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

                memcpy(&((char *) r.data)[colorOfflen.first + i*vertSize], (float *)&COLOR, sizeof(float) * 4);
            }

            fseek(f, normalOffset, SEEK_SET);
            auto normalOfflen = getAttribMetaData(r, PONT_OTHER);

            for (int i = 0; i < normalCount; i++)
            {
                float vertex[3];

                fread(&vertex[0], sizeof(float), 3, f);

                glm::vec3 glmVec = {vertex[0], vertex[1], vertex[2]};

                glmVec = rotation * glm::vec4(glmVec, 1.0f);

                memcpy(&((char *) r.data)[normalOfflen.first + i * vertSize], (float *)&glmVec, sizeof(float) * 3);
            }

            // fill up index buffer
            fseek(f, indexOffset, SEEK_SET);

            for (int i = 0; i < indexCount; i++)
            {
                unsigned short chunk;
                fread(&chunk, sizeof(short), 1, f);
                chunk += r.indexCount;

                unsigned int intChunk = (int) chunk;

                memcpy((char *) r.indices + sizeof(int)*i + r.indexCount, &intChunk, sizeof(int));

                __pAssert(intChunk < (unsigned int) posLength, "Index out of bounds at i=%d.", i);
            }

            fclose(f);

            r.indexCount += indexCount;

            return 1;
        }

        void ModelRenderer::toRData(Renderer::rData &r, unsigned int rOffset, Renderer::vProp property)
        {
            __pWarning("Function `toRData(Renderer::rData, unsigned int, Renderer::vProp)` not implemented yet.");
        }
    }
}

