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
            5.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            5.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            5.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -4.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -4.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -4.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        };
        
        // TODO(HilbertCurve): make this swappable
        Shader::Shader currentShader;
        
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
            
            currentShader = Shader::initShader("./assets/shaders/default.glsl");
            if (currentShader.filepath == nullptr) exit(-1);
            
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
            Shader::attachShader(currentShader);
            // default shader uniforms
            Shader::uploadMat4(currentShader, "uProjection", Camera::getProjection());
            Shader::uploadMat4(currentShader, "uView", Camera::getView());
            
            glBindVertexArray(vaoID);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            
            glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 12 vertices total -> 4 triangles
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindVertexArray(0);
            Shader::detachShader(currentShader);
        }
    }
}