#include "Renderer.h"

#include <GL/gl.h>
#include <stdio.h>

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "GameObject.h"
#include "Rend.h"

namespace Pontilus
{
    namespace Renderer
    {
        GLuint vaoID;
        GLuint vboID;

        static std::vector<Graphics::Rend *> rends;
        
        /*
        static const GLfloat g_vertex_buffer_data[] =
        {
             8.0f,  8.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   //     4
            -1.0f,  8.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   //     5
            -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   //     6
             8.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f    //     7
        };

        static const GLuint g_element_indices[] = 
        {
            3, 2, 0, 0, 2, 1, 7, 6, 4, 4, 6, 5
        };

        static const GLint texSlots[] = {0, 1, 2, 3, 4, 5, 6, 7};

         
         * 1         0
         * 
         * 
         * 
         * 2         3
        

        static GLfloat quad[] = {
             8.0f,  8.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   //     4
            -1.0f,  8.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   //     5
            -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   //     6
             8.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f    //     7
        };
        */
        
        // TODO(HilbertCurve): make this swappable
        Graphics::Shader currentShader;
        Engine::GameObject g;
        Graphics::Rend r;

        void start()
        {
            g = {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
            
            Graphics::initRend(r, 4);

            Engine::gameStateToRend(g, r);
            GLint elementIndices[g.prim.elementSize];
            g.prim.generateIndices(elementIndices, 0);

            //printRend(r);

            glGenVertexArrays(1, &vaoID);
            glBindVertexArray(vaoID);

            // Generate 1 buffer, put the resulting identifier in vboID
            glGenBuffers(1, &vboID);
            // The following commands will talk about our 'vboID' buffer
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            // Give our vertices to OpenGL.
            glBufferData(GL_ARRAY_BUFFER, Graphics::getLayoutLen(r) * r.vertCount, r.data, GL_DYNAMIC_DRAW);

            GLuint eboID;
            glGenBuffers(1, &eboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.prim.elementSize, elementIndices, GL_STATIC_DRAW);
            
            currentShader = Graphics::initShader("./assets/shaders/default.vert", "./assets/shaders/default.frag");
            if (currentShader.vertPath == nullptr || currentShader.fragPath == nullptr) exit(-1);
            
            // TODO(HilbertCurve): automate the glTF file reading process.
            
            // Enable the buffer attribute pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 10, (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 10, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 10, (void*)(7 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(float) * 10, (void*)(9 * sizeof(float)));
            glEnableVertexAttribArray(3);

            //Graphics::initTexture("./assets/textures/cookie.png", square.t);
        }
        
        void render()
        {
            Graphics::attachShader(currentShader);
            // default shader uniforms
            Graphics::uploadMat4(currentShader, "uProjection", Camera::getProjection());
            Graphics::uploadMat4(currentShader, "uView", Camera::getView());
            //Graphics::uploadIntArr(currentShader, "uTextures", texSlots, 8);

            glActiveTexture(GL_TEXTURE1);
            //Graphics::bindTexture(square.t);
            
            glBindVertexArray(vaoID);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);
            glBindVertexArray(0);

            //Graphics::unbindTexture(square.t);

            Graphics::detachShader(currentShader);
        }

        void addRend(Graphics::Rend &r)
        {
            rends.push_back(&r);
        }

        void clean()
        {
            for (int i = 0; i < rends.size(); i++)
            {
                free(rends[i]->data);
                free(rends[i]->layout);
            }
        }
    }
}