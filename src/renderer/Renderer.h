/* date = August 26th 2021 8:40 pm */

#ifndef _PONTILUS_RENDERER_H
#define _PONTILUS_RENDERER_H

#include "renderer/rData.h"
#include "renderer/Texture.h"
#include "renderer/Font.h"
#include "renderer/Shader.h"

#include <vector>
#include <tuple>

namespace Pontilus
{
    namespace Renderer
    {
        /**
         * NOTE: some default targets:
         * 1: rendering quads
         * 2: rendering meshes
         * 3: rendering lines
         * 4: unused
         */
        class RendererController {
            public:
            typedef std::tuple<rData *, Shader *, size_t> Target;

            RendererController(RendererController &other) = delete;

            void registerIconMap(IconMap &im);
            void registerFont(Font &f);

            /**
             * Creates a new target at which one can render.
             */
            size_t registerTarget(size_t vertCount, Primitive *prim, std::string vertPath, std::string fragPath);
            Target getTarget(size_t id);
            void unregisterTarget(size_t id);

            // these should only be used with Application.cpp
            void start();
            void render();
            void close();

            static RendererController &get() {
                if (!inst)
                    inst = new RendererController();
                return *inst;
            }

            ~RendererController() {
                for (auto target : renderTargets) {
                    delete std::get<0>(target);
                    delete std::get<1>(target);
                }
            }

            private:
            void preRender(rData &buffer, Shader &shader);
            void render(rData &buffer, Shader &shader);
            RendererController();

            std::vector<GLuint> iconMapIDs;
            std::vector<GLuint> fontIDs;

            std::vector<Target> renderTargets;

            static RendererController *inst;

            // rData quadPool;
            // Shader quadShader;
            // rData modelPool;
            // Shader modelShader;
            // rData linePool;
            // Shader lineShader;
            // rData fullWindowQuad;
            // rData pointLightPool;
            // Shader postShader;
        };

        /*
        void start();
        void render();
        void modelRender();
        void postRender();
        void debugRender();
        */

        //void renderRData(Renderer::rData &r, Renderer::Primitive mode, unsigned int numObjects);
    }
}

#endif //_PONTILUS_RENDERER_H
