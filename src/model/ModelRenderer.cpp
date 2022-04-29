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

            // fill up rData
            FILE *binFile = fopen(binFP, "rb");


        }
        
        int toRData(Renderer::rData &r, unsigned int rOffset)
        {
            
        }
    }
}

