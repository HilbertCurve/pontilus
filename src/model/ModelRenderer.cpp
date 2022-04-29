#include "model/ModelRenderer.h"

#include "graphics/Renderer.h"
#include "ecs/Component.h"
#include <nlohmann/json.hpp>
#include <string> // std::string::rfind, ::substr, ::c_str

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
            fp = fp.substr(0, lastSlash);

            // get binary name
            std::string bin = gltfData["buffers"][0]["uri"].get<std::string>();
            fp += bin;

            binFP = fp.c_str();

            // get buffer info
            int posAccessor, colorAccessor, indexAccessor;
            auto gltfPrim = gltfData["meshes"][0]["primitives"][0];

            posAccessor = gltfPrim["attributes"]["POSITION"];
            // colorAccessor = gltfPrim["attributes"]["COLOR"];
            indexAccessor = gltfPrim["indices"];

            // for each accessor, get info about buffer layout
            auto accessors = gltfData["accessors"];
            int posLoc = accessors[posAccessor]["bufferView"];
            int indexLoc = accessors[indexAccessor]["bufferView"];

            // get data types
            posType = accessors[posAccessor]["componentType"];
            indexType = accessors[posAccessor]["componentType"];

            posCount = accessors[posAccessor]["count"];
            indexCount = accessors[indexAccessor]["count"];

            // get buffer lengths
            auto bufferViews = gltfData["bufferViews"];

            posLength = bufferViews[posLoc]["byteLength"];
            indexLength = bufferViews[indexLoc]["byteLength"];

            posOffset = bufferViews[posLoc]["byteOffset"];
            indexOffset = bufferViews[indexLoc]["byteOffset"];
        }
        
        int ModelRenderer::toRData(Renderer::rData &r, unsigned int rOffset)
        {
            using namespace Renderer;
            FILE *f = fopen(binFP, "rb");
            if (!f)
            {
                __pWarning("File %s could not be opened.", binFP);
                return 0;
            }

            // fill up pos buffer
            fseek(f, posOffset, SEEK_SET);
            auto offlen = getAttribMetaData(r, PONT_POS);
            int chunkSize = (int) (posLength / posCount);
            int vertSize = getLayoutLen(r);

            for (int i = 0; i < posCount; i++)
            {
                fread(&((char *) r.data)[offlen.first + i * vertSize], chunkSize, 1, f);
            }

            // fill up index buffer
            fseek(f, indexOffset, SEEK_SET);
            chunkSize = (int) (indexLength / posLength);

            for (int i = 0; i < indexCount; i++)
            {
                fread((char *) r.indices + chunkSize*i + r.indexCount, chunkSize, 1, f);
            }

            fclose(f);

            return 1;
        }
    }
}

