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

        static void enableVertexAttribs(Graphics::rData &r)
        {
            int propOffset = 0;
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
            int numObjects = window.scene->objs.size();

            glGenBuffers(1, &vboID);
            glGenBuffers(1, &postvboID);

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
            enableVertexAttribs(*currentRData);
            
            glDrawElements(GL_TRIANGLES, numObjects * 6, GL_UNSIGNED_INT, 0);
            
            disableVertexAttribs(*currentRData);
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
            glBindBuffer(GL_ARRAY_BUFFER, postvboID);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(fullWindowQuad) * fullWindowQuad.vertCount, fullWindowQuad.data, GL_DYNAMIC_DRAW);
            
            Graphics::attachShader(postShader);
            Graphics::uploadFloatArr(postShader, "uLights", (float *) pointLightPool.data, 8 * 4);

            glBindVertexArray(postvaoID);
            enableVertexAttribs(*currentRData);
            
            glDrawElements(GL_TRIANGLES, 1 * 6, GL_UNSIGNED_INT, 0);

            disableVertexAttribs(*currentRData);
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
