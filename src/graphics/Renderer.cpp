#include <GLES3/gl32.h>

#include "Application.h"
#include "Shader.h"
#include "Camera.h"

namespace Pontilus
{
    namespace Renderer
    {
        GLuint vaoID;
        GLuint vboID;
        
        static const GLfloat g_vertex_buffer_data[] =
        {
            -1.0f, -1.0f, 5.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f,  1.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -4.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, -4.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f,  1.0f, -4.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        };
        
        // TODO(HilbertCurve): make this swappable
        Shaders::Shader currentShader;
        
        void start()
        {
            glGenVertexArrays(1, &vaoID);
            glBindVertexArray(vaoID);
            
            // Generate 1 buffer, put the resulting identifier in vboID
            glGenBuffers(1, &vboID);
            // The following commands will talk about our 'vboID' buffer
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            // Give our vertices to OpenGL.
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
            
            currentShader = Shaders::initShader("./assets/shaders/default.glsl");
            
            glVertexAttribPointer(
                                  0,                 // attribute id
                                  3,                 // size
                                  GL_FLOAT,          // type
                                  GL_FALSE,          // normalized?
                                  sizeof(float) * 7, // stride
                                  (void *)0);
            glVertexAttribPointer(
                                  1,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(float) * 7,
                                  (void *)(sizeof(float) * 3));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
        
        void render()
        {
            Shaders::attachShader(currentShader);
            // default shader uniforms
            Shaders::uploadMat4(currentShader, "uProjection", Camera::getProjection());
            Shaders::uploadMat4(currentShader, "uView", Camera::getView());
            
            glBindVertexArray(vaoID);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            
            glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindVertexArray(0);
            Shaders::detachShader(currentShader);
        }
    }
}