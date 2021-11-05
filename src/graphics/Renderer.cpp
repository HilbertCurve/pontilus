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
        GLuint vaoID;
        GLuint vboID;

        static Graphics::rData *currentRData;
        static Graphics::Primitive mode = Graphics::Primitives::QUAD;
          
        static const GLint texSlots[] = {1, 2, 3, 4, 5, 6, 7, 8};
        static Graphics::Texture *textures[8] = 
        {
            {}, {}, {}, {}, {}, {}, {}, {}
        };
        
        // TODO: make this swappable
        Graphics::Shader currentShader;

        static void setRData(Graphics::rData &r)
        {
            currentRData = &r;
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(*currentRData) * currentRData->vertCount, currentRData->data, GL_DYNAMIC_DRAW);
        }

        Graphics::rData r;
        void start()
        {
            int numObjects = window.scene->objs.size();

            // Generate 1 buffer, put the resulting identifier in vboID
            glGenBuffers(1, &vboID);

            GLint elementIndices[mode.elementSize * numObjects];
            for (int i = 0; i < numObjects; i++)
            {
                mode.generateIndices(elementIndices, i);
            }
            //GLint elementIndices[] = {3, 2, 0, 0, 2, 1};

            setRData(rDataPool);

            printRData(*currentRData, numObjects * 4);

            glGenVertexArrays(1, &vaoID);
            glBindVertexArray(vaoID);

            GLuint eboID;
            glGenBuffers(1, &eboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndices), elementIndices, GL_STATIC_DRAW);
            
            currentShader = Graphics::initShader("./assets/shaders/default.vert", "./assets/shaders/default.frag");
            if (currentShader.vertPath == nullptr || currentShader.fragPath == nullptr) exit(-1);
            
            // TODO(HilbertCurve): automate the glTF file reading process.
            
            // Enable the buffer attribute pointers
            // TODO: automate vertex attributes
            glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 10, (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 10, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 10, (void*)(7 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(float) * 10, (void*)(9 * sizeof(float)));
            glEnableVertexAttribArray(3);
        }
        
        void render()
        {
            if (currentRData->isDirty)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vboID);
                glBufferSubData(GL_ARRAY_BUFFER, 0, getLayoutLen(*currentRData) * 4, currentRData->data); // automate me
            }

            int numObjects = window.scene->objs.size();

            Graphics::attachShader(currentShader);
            // default shader uniforms
            Graphics::uploadMat4(currentShader, "uProjection", Camera::getProjection());
            Graphics::uploadMat4(currentShader, "uView", Camera::getView());
            Graphics::uploadIntArr(currentShader, "uTextures", texSlots, 8);
            Graphics::uploadFloat(currentShader, "uTime", (float) glfwGetTime());

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

            Graphics::detachShader(currentShader);
        }
    }
}
