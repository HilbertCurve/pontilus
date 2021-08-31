#include <GLES3/gl32.h>

#include "Application.h"
#include "Shader.h"

namespace Application
{
    namespace Renderer
    {
        GLuint vaoID;
        GLuint vboID;
        
        static const GLfloat g_vertex_buffer_data[] = 
        {
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        };
        
        // TODO: make this swappable
        Shader currentShader;
        
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
            
            currentShader = initShader("/home/ben/c/game/assets/shaders/default.glsl");
            
            glVertexAttribPointer(
                                  0,                  // attribute id
                                  3,                  // size
                                  GL_FLOAT,           // type
                                  GL_FALSE,           // normalized?
                                  sizeof(float)*7,    // stride
                                  (void*)0  
                                  );
            glVertexAttribPointer(
                                  1,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(float)*7,
                                  (void*)(sizeof(float)*3)
                                  );
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
        
        void update() 
        {
            attachShader(currentShader);
            glBindVertexArray(vaoID);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            
            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindVertexArray(0);
            detachShader(currentShader);
        }
    }
}