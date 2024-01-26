#include "renderer/Renderer.h"

#include <glad/gl.h>
#include <stdio.h>

#include "core/Application.h"
#include "renderer/Shader.h"
#include "renderer/Camera.h"
#include "renderer/Texture.h"
#include "renderer/Font.h"
#include "renderer/rData.h"
#include "renderer/Primitive.h"

namespace Pontilus
{
    namespace Renderer
    {

/*
        Renderer::Shader gameShader;
        Renderer::Shader modelShader;
        Renderer::Shader postShader;
        Renderer::Shader debugShader;
*/

        // these are private functions, only for renderer code
        static void enableVertexAttribs(Renderer::rData &r)
        {
            u_int64_t propOffset = 0;
            for (unsigned int i = 0; i < r.layoutCount; i++)
            {
                int propertyLen = r.layout[i].count;
                Renderer::vPropType type = r.layout[i].type;

                glVertexAttribPointer(i, propertyLen, GL_FLOAT, false, r.getLayoutLen(), (void *)propOffset);
                glEnableVertexAttribArray(i);

                propOffset += propertyLen * Renderer::getVTypeLen(type);
            }
        }

        static void disableVertexAttribs(Renderer::rData &r)
        {
            for (unsigned int i = 0; i < r.layoutCount; i++)
            {
                glDisableVertexAttribArray(i);
            }
        }

        static void initRDataGLCaps(rData &r)
        {
            glGenBuffers(1, &r.vbo);
            glGenBuffers(1, &r.ebo);

            glGenVertexArrays(1, &r.vao);

            glBindVertexArray(r.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    r.indexCount * sizeof(unsigned int),
                    r.indices,
                    GL_DYNAMIC_DRAW);
        }

        RendererController *RendererController::inst = nullptr;

        const size_t RendererController::QUAD_TARGET = 1;
        const size_t RendererController::MESH_TARGET = 2;
        const size_t RendererController::LINE_TARGET = 3;
        const size_t RendererController::FULL_WINDOW_TARGET = 4;

        RendererController::RendererController() {
            this->renderTargets.clear();

            // IMPORTANT: keep these targets in exactly this order; we want to keep parity with static render target ids
            this->registerTarget(40000, &Primitives::QUAD, std::string("../assets/") + "shaders/default.vert", std::string("../assets/") + "shaders/default.frag");
            this->registerTarget(4000, &Primitives::MESH, std::string("../assets/") + "shaders/model.vert", std::string("../assets/") + "shaders/model.frag");
            this->registerTarget(400, &Primitives::LINE, std::string("../assets/") + "shaders/debug.vert", std::string("../assets/") + "shaders/debug.frag");
            this->registerTarget(4, &Primitives::QUAD, std::string("../assets/") + "shaders/post.vert", std::string("../assets/") + "shaders/post.frag");

            this->iconMapIDs.clear();
            this->fontIDs.clear();
        }

        size_t RendererController::registerTarget(
            size_t vertCount,
            Primitive *prim,
            std::string vertPath,
            std::string fragPath)
        {
            static size_t id = 0;
            id++;

            rData *d = new rData(vertCount, prim);
            Shader *s = new Shader(vertPath, fragPath);

            this->preRender(*d, *s);

            this->renderTargets.push_back(std::make_tuple(d, s, id));
            return id;
        }

        RendererController::Target RendererController::getTarget(size_t id) {
            for (auto target : renderTargets) {
                if (std::get<2>(target) == id) {
                    return target;
                }
            }

            __pWarning("Render target of id %lu not found.", id);
            return std::make_tuple(nullptr, nullptr, 0);
        }

        void RendererController::unregisterTarget(size_t id) {
            for (size_t i = 0; i < renderTargets.size(); i++) {
                if (std::get<2>(renderTargets[i]) == id) {
                    renderTargets.erase(renderTargets.begin() + i);
                    return;
                }
            }

            __pWarning("Render target of id %lu not found.", id);
        }

        void RendererController::registerIconMap(IconMap &im) {
            this->iconMapIDs.push_back(im.id());
        }

        void RendererController::registerFont(Font &f) {
            this->fontIDs.push_back(f.id());
        }

        void RendererController::start() {
            for (auto &target : renderTargets) {
                this->preRender(*std::get<0>(target), *std::get<1>(target));
            }
        }

        void RendererController::close() {
            for (auto &target : renderTargets) {
                std::get<1>(target)->detach();

                delete std::get<0>(target);
                delete std::get<1>(target);
            }

            this->iconMapIDs.clear();
            this->fontIDs.clear();
            this->renderTargets.clear();
        }

        void RendererController::preRender(rData &buffer, Shader &shader)
        {
            glGenBuffers(1, &buffer.vbo);
            glGenBuffers(1, &buffer.ebo);
            
            // GLint elementIndices[] = {3, 2, 0, 0, 2, 1};

            glGenVertexArrays(1, &buffer.vao);

            glBindVertexArray(buffer.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    buffer.indexCount * sizeof(unsigned int), 
                    buffer.indices, 
                    GL_DYNAMIC_DRAW);

            initRDataGLCaps(buffer);

            /*
            gameShader = initShader(getDefaultShader(true), getDefaultShader(false));
            modelShader = initShader("../assets/shaders/model.vert", "../assets/shaders/model.frag");
            postShader = initShader("../assets/shaders/post.vert", "../assets/shaders/post.frag");
            debugShader = initShader("../assets/shaders/debug.vert", "../assets/shaders/debug.frag");
            */
        }

        static const int texSlots[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

        void RendererController::render(size_t targetID, Camera &camera)
        {
            auto full = this->getTarget(targetID);
            auto &buffer = *std::get<0>(full);
            auto &shader = *std::get<1>(full);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
            glBufferData(GL_ARRAY_BUFFER, buffer.dataOffset, buffer.data, GL_DYNAMIC_DRAW);
            // regenerate element buffer
            glBindVertexArray(buffer.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    buffer.indexCount * sizeof(unsigned int),
                    buffer.indices,
                    GL_DYNAMIC_DRAW);

            shader.attach();
            // default shader uniforms

            // TODO: make these members of shader
            shader.uploadMat4("uProjection", camera.getProjection());
            shader.uploadMat4("uView", camera.getView());
            shader.uploadIntArr("uTextures", texSlots, 16);
            shader.uploadFloat("uTime", (float)glfwGetTime());

            for (GLuint id : this->iconMapIDs)
            {
                if (id == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + id);
                glBindTexture(GL_TEXTURE_2D, id);
            }

            for (GLuint id : this->fontIDs)
            {
                if (id == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + id);
                glBindTexture(GL_TEXTURE_2D, id);
            }

            glBindVertexArray(buffer.vao);
            enableVertexAttribs(buffer);

            glDrawElements(GL_TRIANGLES, buffer.indexCount, GL_UNSIGNED_INT, 0);

            disableVertexAttribs(buffer);
            glBindVertexArray(0);

            for (GLuint id : this->iconMapIDs)
            {
                if (id == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + id);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            for (GLuint id : this->fontIDs)
            {
                if (id == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + id);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            // reset the offset; time to reload data!
            buffer.dataOffset = 0;

            shader.detach();
        }
    }
}

