#include "Renderer.h"

#include <GL/gl.h>
#include <stdio.h>

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "GameObject.h"
#include "rData.h"
#include "Primitive.h"

namespace Pontilus
{
    namespace Renderer
    {
        GLuint vaoID, postvaoID;
        GLuint vboID, postvboID;
        GLuint eboID, posteboID;

        static Graphics::rData *currentRData;
        static Graphics::Primitive mode = Graphics::Primitives::QUAD;

        static Graphics::rData fullWindowQuad;
        static Graphics::vAttrib fullWindowQuadAttribs[2] = 
        {
            Graphics::vAttrib
            {
                Graphics::PONT_POS, Graphics::PONT_FLOAT, 3
            },
            Graphics::vAttrib
            {
                Graphics::PONT_COLOR, Graphics::PONT_FLOAT, 4
            }
        };
          
        static const GLint texSlots[] = {1, 2, 3, 4, 5, 6, 7, 8};
        static Graphics::Texture *textures[8] = 
        {
            {}, {}, {}, {}, {}, {}, {}, {}
        };
        
        // TODO: make this swappable
        Graphics::Shader gameShader;
        Graphics::Shader postShader;

        static void setRData(Graphics::rData &r)
        {
            currentRData = &r;
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(*currentRData) * currentRData->vertCount, currentRData->data, GL_DYNAMIC_DRAW);
            
        }

        static void setPrimitive(Graphics::Primitive p)
        {
            mode = p;

            int numElements = 10;

            switch (p.renderMode)
            {
                case PONT_GAME:
                {
                    numElements = window.scene->objs.size();
                } break;
                case PONT_DEBUG:
                {
                    numElements = 1; // automate me
                } break;
            }
            printf("%d\n", numElements * mode.elementSize);

            GLint elementIndices[mode.elementSize * numElements];
            for (int i = 0; i < numElements; i++)
            {
                mode.generateIndices(elementIndices, i);
            }

            GLuint eboID;
            glGenBuffers(1, &eboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndices), elementIndices, GL_STATIC_DRAW);
        }

        Graphics::rData r;
        void start()
        {
            int numObjects = window.scene->objs.size();

            glGenBuffers(1, &vboID);

            //GLint elementIndices[] = {3, 2, 0, 0, 2, 1};

            setRData(quadPool);

            glGenVertexArrays(1, &vaoID);
            glBindVertexArray(vaoID);

            setPrimitive(Graphics::Primitives::QUAD);
            
            gameShader = Graphics::initShader("./assets/shaders/default.vert", "./assets/shaders/default.frag");
            if (gameShader.vertPath == nullptr || gameShader.fragPath == nullptr) exit(-1);

            int propOffset = 0;
            for (int i = 0; i < currentRData->layoutCount; i++)
            {
                int propertyLen = currentRData->layout[i].count;
                Graphics::vPropType type = currentRData->layout[i].type;
                
                glVertexAttribPointer(i, propertyLen, GL_FLOAT, false, getLayoutLen(*currentRData), (void *)propOffset);
                glEnableVertexAttribArray(i);

                propOffset += propertyLen * Graphics::getVTypeLen(type);
            }
        }
        
        void render()
        {
            if (currentRData->isDirty)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vboID);
                glBufferSubData(GL_ARRAY_BUFFER, 0, getLayoutLen(*currentRData) * 4, currentRData->data); // automate me
            }

            int numObjects = window.scene->objs.size();

            Graphics::attachShader(gameShader);
            // default shader uniforms
            Graphics::uploadMat4(gameShader, "uProjection", Camera::getProjection());
            Graphics::uploadMat4(gameShader, "uView", Camera::getView());
            Graphics::uploadIntArr(gameShader, "uTextures", texSlots, 8);
            Graphics::uploadFloat(gameShader, "uTime", (float) glfwGetTime());

            for (int i = 0; i < sizeof(texPool)/sizeof(Graphics::Texture *); i++)
            {
                if (texPool[i] == nullptr) continue;

                glActiveTexture(GL_TEXTURE0 + i + 1);
                Graphics::bindTexture(*texPool[i]);
            }
            
            glBindVertexArray(vaoID);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            
            glDrawElements(GL_TRIANGLES, numObjects * 6, GL_UNSIGNED_INT, 0);
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);
            glBindVertexArray(0);

            for (int i = 0; i < sizeof(texPool)/sizeof(Graphics::Texture *); i++)
            {
                if (texPool[i] == nullptr) continue;
                
                glActiveTexture(GL_TEXTURE0 + i + 1);
                Graphics::unbindTexture(*texPool[i]);
            }

            Graphics::detachShader(gameShader);

            /*for (int i = 0; i < currentRData->layoutCount; i++)
            {
                int propertyLen = currentRData->layout[i].count;
                Graphics::vPropType type = currentRData->layout[i].type;
                
                glVertexAttribPointer(i, propertyLen, GL_FLOAT, false, getLayoutLen(*currentRData), (void *)propOffset);
                glEnableVertexAttribArray(i);

                propOffset += propertyLen * Graphics::getVTypeLen(type);
            }*/

            
        }

        void postRender()
        {
            setRData(fullWindowQuad);

            setPrimitive(Graphics::Primitives::QUAD);

            Graphics::attachShader(postShader);
            Graphics::uploadFloatArr(postShader, "uLights", (float *) pointLightPool.data, 8 * 4);

            //glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 10, (void*)0);
            //glEnableVertexAttribArray(0);

            //glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 10, (void*)(3 * sizeof(float)));
            //glEnableVertexAttribArray(1); // automate me

            glBindVertexArray(postvaoID);
            
            //glDrawElements(GL_TRIANGLES, 1 * 6, GL_UNSIGNED_INT, 0);

            //glDisableVertexAttribArray(0);
            //glDisableVertexAttribArray(1);

            glBindVertexArray(0);

            Graphics::detachShader(postShader);
        }

        void renderRData(Graphics::rData &r, Graphics::Primitive mode, unsigned int numObjects)
        {
            setRData(r);

            //setPrimitive(mode, gameeboID);


        }
    }
}
