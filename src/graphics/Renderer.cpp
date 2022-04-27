#include "graphics/Renderer.h"

#include <GL/gl.h>
#include <stdio.h>

#include "core/Application.h"
#include "graphics/Shader.h"
#include "graphics/Camera.h"
#include "graphics/Texture.h"
#include "graphics/Font.h"
#include "graphics/rData.h"
#include "graphics/Primitive.h"

namespace Pontilus
{
    namespace Renderer
    {
        // quad pool:
        Renderer::rData quadPool = {};

        // debug line pool:
        Renderer::rData linePool = {};
        Renderer::vAttrib linePoolAttribs[2] = 
        {
            { Renderer::PONT_POS, Renderer::PONT_FLOAT, 3 },
            { Renderer::PONT_COLOR, Renderer::PONT_FLOAT, 4 }
        };

        // fullScreenQuad
        Renderer::rData fullWindowQuad = {};
        Renderer::vAttrib fullWindowQuadAttribs[2] = 
        {
            { Renderer::PONT_POS, Renderer::PONT_FLOAT, 3 },
            { Renderer::PONT_COLOR, Renderer::PONT_FLOAT, 4 }
        };

        static void initQuads()
        {
            Renderer::initRData(quadPool, 4000, &Primitives::QUAD);

            Renderer::initRData(fullWindowQuad, 4, &Primitives::QUAD, fullWindowQuadAttribs, 2);
            glm::vec3 orientation;

            for (int i = 0; i < 4; i++)
            {
                float r = Renderer::Camera::projectionWidth/2;
                float u = Renderer::Camera::projectionHeight/2;
                switch (i)
                {
                    case 0: orientation = { r,  u, 0.0f}; break;
                    case 1: orientation = {-r,  u, 0.0f}; break;
                    case 2: orientation = {-r, -u, 0.0f}; break;
                    case 3: orientation = { r, -u, 0.0f}; break;
                }
                for (int j = 0; j < 3; j++)
                {
                    ((float *)fullWindowQuad.data)[i * 7 + j] = orientation[j];
                }

                for (int j = 0; j < 4; j++)
                {
                    ((float *)fullWindowQuad.data)[i * 7 + j + 3] = 0.00f;
                }
            }
        }

        static void initLines()
        {
            Renderer::initRData(linePool, 1000, &Primitives::LINE, linePoolAttribs, 2);
        }

        static void cleanQuads()
        {
            free(quadPool.data);
            free(quadPool.layout);
        }

        static void cleanLines()
        {
            free(linePool.data);
            free(linePool.layout);
        }

        // pointLight pool
        Renderer::rData pointLightPool = {};
        static Renderer::vAttrib pointLightAttributes[3] = 
        {
            { Renderer::PONT_POS,   Renderer::PONT_FLOAT, 3 },
            { Renderer::PONT_COLOR, Renderer::PONT_FLOAT, 4 },
            { Renderer::PONT_OTHER, Renderer::PONT_FLOAT, 1 }
        };

        static void initPointLights()
        {
            Renderer::initRData(pointLightPool, 16, &Primitives::NONE, pointLightAttributes, 3);
        }

        static void cleanPointLights()
        {
            free(pointLightPool.data);
            free(pointLightPool.layout);
        }

        // Texture pool:
        Renderer::IconMap *iconPool[8];
        int iconPoolStackPointer = 0;

        static void initTexPool()
        {
            for (int i = 0; i < 8; i++)
            {
                iconPool[i] = nullptr;
            }
        }

        static void cleanTexPool()
        {
            for (int i = 0; i < 8; i++)
            {
                iconPool[i] = nullptr;
            }
        }

        Renderer::Font *fontPool[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int fontPoolStackPointer = 0;

        static Renderer::rData *currentRData;

        static const GLint texSlots[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

        Renderer::Shader gameShader;
        Renderer::Shader postShader;
        Renderer::Shader debugShader;

        static void setRData(Renderer::rData &r)
        {
            currentRData = &r;
        }

        static void enableVertexAttribs(Renderer::rData &r)
        {
            u_int64_t propOffset = 0;
            for (unsigned int i = 0; i < r.layoutCount; i++)
            {
                int propertyLen = r.layout[i].count;
                Renderer::vPropType type = r.layout[i].type;

                glVertexAttribPointer(i, propertyLen, GL_FLOAT, false, getLayoutLen(r), (void *)propOffset);
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

        void start()
        {
            initQuads();
            initPointLights();
            initLines();
            initTexPool();

            glGenBuffers(1, &quadPool.vbo);
            glGenBuffers(1, &fullWindowQuad.vbo);
            glGenBuffers(1, &linePool.vbo);

            glGenBuffers(1, &quadPool.ebo);
            glGenBuffers(1, &fullWindowQuad.ebo);
            glGenBuffers(1, &linePool.ebo);

            // GLint elementIndices[] = {3, 2, 0, 0, 2, 1};

            glGenVertexArrays(1, &quadPool.vao);
            glGenVertexArrays(1, &fullWindowQuad.vao);
            glGenVertexArrays(1, &linePool.ebo);

            glBindVertexArray(quadPool.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadPool.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    quadPool.indexCount * sizeof(unsigned int),
                    quadPool.indices,
                    GL_DYNAMIC_DRAW);

            glBindVertexArray(fullWindowQuad.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fullWindowQuad.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    fullWindowQuad.indexCount * sizeof(unsigned int), 
                    fullWindowQuad.indices, 
                    GL_DYNAMIC_DRAW);

            glBindVertexArray(linePool.ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linePool.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    linePool.indexCount * sizeof(unsigned int),
                    linePool.indices,
                    GL_DYNAMIC_DRAW);

            gameShader = initShader("./assets/shaders/default.vert", "./assets/shaders/default.frag");
            postShader = initShader("./assets/shaders/pointmap.vert", "./assets/shaders/pointmap.frag");
            debugShader = initShader("./assets/shaders/debug.vert", "./assets/shaders/debug.frag");
        }

        void render()
        {
            setRData(quadPool);

            glBindBuffer(GL_ARRAY_BUFFER, quadPool.vbo);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(*currentRData) * currentRData->vertCount, currentRData->data, GL_DYNAMIC_DRAW);

            int numObjects = 0;

            if (getCurrentScene() != nullptr)
            {
                // TODO: phase this out; rData sizes are dynamic now.
                numObjects = getCurrentScene()->numQuads;
            }
            // regenerate element buffer
            glBindVertexArray(quadPool.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadPool.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    quadPool.indexCount * sizeof(unsigned int),
                    quadPool.indices,
                    GL_DYNAMIC_DRAW);

            Renderer::attachShader(gameShader);
            // default shader uniforms
            Renderer::uploadMat4(gameShader, "uProjection", Camera::getProjection());
            Renderer::uploadMat4(gameShader, "uView", Camera::getView());
            Renderer::uploadIntArr(gameShader, "uTextures", texSlots, 16);
            Renderer::uploadFloat(gameShader, "uTime", (float)glfwGetTime());

            for (int i = 0; i < 8; i++)
            {
                if (iconPool[i] == nullptr || iconPool[i]->texID == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + iconPool[i]->texID);
                Renderer::bindIconMap(*iconPool[i]);
            }

            for (int i = 0; i < 8; i++)
            {
                if (fontPool[i] == nullptr || fontPool[i]->texID == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + fontPool[i]->texID);
                Renderer::bindFont(*fontPool[i]);
            }

            glBindVertexArray(quadPool.vao);
            enableVertexAttribs(*currentRData);

            glDrawElements(GL_TRIANGLES, numObjects * 6, GL_UNSIGNED_INT, 0);

            disableVertexAttribs(*currentRData);
            glBindVertexArray(0);

            for (int i = 0; i < 8; i++)
            {
                if (iconPool[i] == nullptr || iconPool[i]->texID == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + iconPool[i]->texID);
                Renderer::unbindIconMap(*iconPool[i]);
            }

            for (int i = 0; i < 8; i++)
            {
                if (fontPool[i] == nullptr || fontPool[i]->texID == 0.0f)
                    continue;

                glActiveTexture(GL_TEXTURE0 + fontPool[i]->texID);
                Renderer::unbindFont(*fontPool[i]);
            }

            Renderer::detachShader(gameShader);
        }

        void postRender()
        {
            setRData(fullWindowQuad);
            glBindBuffer(GL_ARRAY_BUFFER, fullWindowQuad.vbo);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(fullWindowQuad) * fullWindowQuad.vertCount, fullWindowQuad.data, GL_DYNAMIC_DRAW);

            glBindVertexArray(fullWindowQuad.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fullWindowQuad.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    fullWindowQuad.indexCount * sizeof(unsigned int), 
                    fullWindowQuad.indices, 
                    GL_DYNAMIC_DRAW);

            Renderer::attachShader(postShader);

            float lightPos[4 * 3];
            float lightColor[4 * 4];
            float lightIntensity[4 * 1];

            for (int i = 0; i < 4; i++)
            {
                // position
                for (int j = 0; j < 3; j++)
                {
                    lightPos[i * 3 + j] = ((float *)pointLightPool.data)[i * 8 + j];
                }

                // color
                for (int j = 0; j < 4; j++)
                {
                    lightColor[i * 4 + j] = ((float *)pointLightPool.data)[i * 8 + j + 3];
                }

                // intensity
                lightIntensity[i] = ((float *)pointLightPool.data)[i * 8 + 7];
            }

            // printf("%f\n", lightIntensity[0]);

            Renderer::uploadMat4(postShader, "uProjection", Camera::getProjection());
            Renderer::uploadMat4(postShader, "uView", Camera::getView());
            Renderer::uploadVec2Arr(postShader, "uCameraPos", (float *)&Camera::getPosition(), 2);
            // printf("%f\n", Renderer::Camera::getPosition().x);

            Renderer::uploadVec3Arr(postShader, "uLightPos", lightPos, 3 * 4);
            Renderer::uploadVec4Arr(postShader, "uLightColor", lightColor, 4 * 4);
            Renderer::uploadFloatArr(postShader, "uLightIntensity", lightIntensity, 4);

            // there's probably an alternative here that I could use involving matrices, but oh well
            Renderer::uploadVec2Arr(postShader, "uResolution", &resolution, 1);

            glBindVertexArray(fullWindowQuad.vbo);
            enableVertexAttribs(*currentRData);

            glDrawElements(GL_TRIANGLES, 1 * 6, GL_UNSIGNED_INT, 0);

            disableVertexAttribs(*currentRData);
            glBindVertexArray(0);

            Renderer::detachShader(postShader);
        }

        void debugRender()
        {
            setRData(linePool);

            glBindBuffer(GL_ARRAY_BUFFER, linePool.vbo);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(linePool) * linePool.vertCount, linePool.data, GL_DYNAMIC_DRAW);

            glBindVertexArray(linePool.ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linePool.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    linePool.indexCount * sizeof(unsigned int),
                    linePool.indices,
                    GL_DYNAMIC_DRAW);

            Renderer::attachShader(debugShader);

            Renderer::uploadMat4(debugShader, "uProjection", Camera::getProjection());
            Renderer::uploadMat4(debugShader, "uView", Camera::getView());

            glBindVertexArray(linePool.vbo);
            enableVertexAttribs(*currentRData);

            glDrawElements(GL_LINES_ADJACENCY, 1 * 6, GL_UNSIGNED_INT, 0);

            disableVertexAttribs(*currentRData);
            glBindVertexArray(0);

            Renderer::detachShader(debugShader);
        }

        void close()
        {
            cleanQuads();
            cleanPointLights();
            cleanLines();
            cleanTexPool();
        }
    }
}

