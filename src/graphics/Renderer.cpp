#include "Renderer.h"

#include <GL/gl.h>
#include <stdio.h>

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "GameObject.h"
#include "Rend.h"
#include "Primitive.h"

namespace Pontilus
{
    namespace Renderer
    {
        GLuint vaoID;
        GLuint vboID;

        static std::vector<Graphics::Rend *> rends;
        static Graphics::Rend currentRend;
        static Graphics::Primitive mode = Graphics::Primitives::QUAD;
          
        static const GLint texSlots[] = {1, 2, 3, 4, 5, 6, 7, 8};
        static Graphics::Texture textures[2];
        
        // TODO(HilbertCurve): make this swappable
        Graphics::Shader currentShader;

        static void setRend(Graphics::Rend *r)
        {
            currentRend = *r;
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferData(GL_ARRAY_BUFFER, getLayoutLen(currentRend) * currentRend.vertCount, currentRend.data, GL_DYNAMIC_DRAW);
        }

        Graphics::Rend r;
        void start()
        {
            // Generate 1 buffer, put the resulting identifier in vboID
            glGenBuffers(1, &vboID);

            setRend(rends[0]);

            Graphics::initTexture("./assets/textures/ghostSwole.png", textures[0]);
            Graphics::initTexture("./assets/textures/cookie.png", textures[1]);

            GLint elementIndices[mode.elementSize * 2];
            mode.generateIndices(elementIndices, 0);
            mode.generateIndices(elementIndices, 1);
            //GLint elementIndices[] = {3, 2, 0, 0, 2, 1};

            printRend(currentRend);

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
            Graphics::attachShader(currentShader);
            // default shader uniforms
            Graphics::uploadMat4(currentShader, "uProjection", Camera::getProjection());
            Graphics::uploadMat4(currentShader, "uView", Camera::getView());
            Graphics::uploadInt(currentShader, "uTexture1", 0);
            Graphics::uploadInt(currentShader, "uTexture2", 1);
            Graphics::uploadIntArr(currentShader, "uTextures", texSlots, 8);
            
            Graphics::Texture t1;
            Graphics::Texture t2;
            Graphics::initTexture("./assets/textures/ghostSwole.png", t1);
            Graphics::initTexture("./assets/textures/cookie.png", t2);
            

            for (int i = 0; i < sizeof(textures)/sizeof(Graphics::Texture); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i + 1);
                Graphics::bindTexture(textures[i]);
            }
            
            glBindVertexArray(vaoID);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);
            glBindVertexArray(0);

            for (int i = 0; i < sizeof(textures)/sizeof(Graphics::Texture); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i + 1);
                Graphics::unbindTexture(textures[i]);
            }

            Graphics::detachShader(currentShader);
        }

        void addRend(Graphics::Rend &r)
        {
            rends.push_back(&r);
        }
    }
}