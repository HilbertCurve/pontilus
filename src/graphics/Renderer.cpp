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
        // TODO: abstract this away
        GLuint vaoID, postvaoID, debugvaoID;
        GLuint vboID, postvboID, debugvboID;
        GLuint eboID, posteboID, debugeboID;

        static Graphics::rData *currentRData;
        static Graphics::Primitive currentMode = Graphics::Primitives::QUAD;
          
        static const GLint texSlots[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        
        Graphics::Shader gameShader;
        Graphics::Shader postShader;
        Graphics::Shader debugShader;

        static void setRData(Graphics::rData &r)
        {
            currentRData = &r;            
        }

        static void setPrimitive(Graphics::Primitive p)
        {
            currentMode = p;

            int numElements = 0;

            switch (p.renderMode)
            {
                case PONT_GAME:
                {
                    if (getCurrentScene() != nullptr)
                    {
                        numElements = getCurrentScene()->numQuads;
                    }
                } break;
                case PONT_DEBUG:
                {
                    numElements = 1; // automate me
                } break;
            }

            GLint elementIndices[currentMode.elementSize * numElements];
            for (int i = 0; i < numElements; i++)
            {
                currentMode.generateIndices(elementIndices, i);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndices), elementIndices, GL_DYNAMIC_DRAW);
        }

        static void enableVertexAttribs(Graphics::rData &r)
        {
            u_int64_t propOffset = 0;
            for (int i = 0; i < r.layoutCount; i++)
            {
                int propertyLen = r.layout[i].count;
                Graphics::vPropType type = r.layout[i].type;
                
                glVertexAttribPointer(i, propertyLen, GL_FLOAT, false, getLayoutLen(r), (void *)propOffset);
                glEnableVertexAttribArray(i);

                propOffset += propertyLen * Graphics::getVTypeLen(type);
            }
        }

        static void disableVertexAttribs(Graphics::rData &r)
        {
            for (int i = 0; i < r.layoutCount; i++)
            {
                glDisableVertexAttribArray(i);
            }
        }

        void start()
        {
            glGenBuffers(1, &vboID);
            glGenBuffers(1, &postvboID);

            glGenBuffers(1, &eboID);

            //GLint elementIndices[] = {3, 2, 0, 0, 2, 1};

            glGenVertexArrays(1, &vaoID);
            glGenVertexArrays(1, &postvaoID);
            glBindVertexArray(vaoID);
            setPrimitive(Graphics::Primitives::QUAD);
            glBindVertexArray(postvaoID);
            setPrimitive(Graphics::Primitives::QUAD);
            //glBindVertexArray(vaoID);
            
            gameShader = Graphics::initShader("./assets/shaders/default.vert", "./assets/shaders/default.frag");
            postShader = Graphics::initShader("./assets/shaders/pointmap.vert", "./assets/shaders/pointmap.frag");

            //enableVertexAttribs(*currentRData);
        }
        
        void render()
        {
            setRData(quadPool);
            
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(*currentRData) * currentRData->vertCount, currentRData->data, GL_DYNAMIC_DRAW);

            int numObjects = 0;

            if (getCurrentScene() != nullptr)
            {
                numObjects = getCurrentScene()->numQuads;
            }
            setPrimitive(Pontilus::Graphics::Primitives::QUAD);
            

            Graphics::attachShader(gameShader);
            // default shader uniforms
            Graphics::uploadMat4(gameShader, "uProjection", Camera::getProjection());
            Graphics::uploadMat4(gameShader, "uView", Camera::getView());
            Graphics::uploadIntArr(gameShader, "uTextures", texSlots, 16);
            Graphics::uploadFloat(gameShader, "uTime", (float) glfwGetTime());

            for (int i = 0; i < 8; i++)
            {
               if (iconPool[i] == nullptr || iconPool[i]->texID == 0.0f) continue;

               glActiveTexture(GL_TEXTURE0 + iconPool[i]->texID);
               Graphics::bindIconMap(*iconPool[i]);
            }

            for (int i = 0; i < 8; i++)
            {
                if (fontPool[i] == nullptr || fontPool[i]->texID == 0.0f) continue;

                glActiveTexture(GL_TEXTURE0 + fontPool[i]->texID);
                Graphics::bindFont(*fontPool[i]);
            }
            
            glBindVertexArray(vaoID);
            enableVertexAttribs(*currentRData);
            
            glDrawElements(GL_TRIANGLES, numObjects * 6, GL_UNSIGNED_INT, 0);
            
            disableVertexAttribs(*currentRData);
            glBindVertexArray(0);

            for (int i = 0; i < 8; i++)
            {
               if (iconPool[i] == nullptr || iconPool[i]->texID == 0.0f) continue;

               glActiveTexture(GL_TEXTURE0 + iconPool[i]->texID);
               Graphics::unbindIconMap(*iconPool[i]);
            }

            for (int i = 0; i < 8; i++)
            {
                if (fontPool[i] == nullptr || fontPool[i]->texID == 0.0f) continue;

                glActiveTexture(GL_TEXTURE0 + fontPool[i]->texID);
                Graphics::unbindFont(*fontPool[i]);
            }

            Graphics::detachShader(gameShader);
        }

        void postRender()
        {
            setRData(fullWindowQuad);
            glBindBuffer(GL_ARRAY_BUFFER, postvboID);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(fullWindowQuad) * fullWindowQuad.vertCount, fullWindowQuad.data, GL_DYNAMIC_DRAW);
            
            Graphics::attachShader(postShader);
            
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

            //printf("%f\n", lightIntensity[0]);

            Graphics::uploadMat4(postShader, "uProjection", Camera::getProjection());
            Graphics::uploadMat4(postShader, "uView", Camera::getView());
            Graphics::uploadVec2Arr(postShader, "uCameraPos", (float *)&Camera::getPosition(), 2);
            //printf("%f\n", Renderer::Camera::getPosition().x);

            Graphics::uploadVec3Arr(postShader, "uLightPos", lightPos, 3 * 4);
            Graphics::uploadVec4Arr(postShader, "uLightColor", lightColor, 4 * 4);
            Graphics::uploadFloatArr(postShader, "uLightIntensity", lightIntensity, 4);

            // there's probably an alternative here that I could use involving matrices, but oh well
            Graphics::uploadVec2Arr(postShader, "uResolution", &resolution, 1);

            glBindVertexArray(postvaoID);
            enableVertexAttribs(*currentRData);
            
            glDrawElements(GL_TRIANGLES, 1 * 6, GL_UNSIGNED_INT, 0);

            disableVertexAttribs(*currentRData);
            glBindVertexArray(0);

            Graphics::detachShader(postShader);
        }
    }
}
