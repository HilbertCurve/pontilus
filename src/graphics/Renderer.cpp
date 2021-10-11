#include "Renderer.h"

#include <GLES3/gl32.h>
#include <stdio.h>

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Obj3D.h"

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
        Model::Obj3D test;
        
        void start()
        {
            glGenVertexArrays(1, &vaoID);
            glBindVertexArray(vaoID);
            
            Model::loadObjFromBinary("./assets/models/monkee.bin", test);

            // Generate 1 buffer, put the resulting identifier in vboID
            glGenBuffers(1, &vboID);
            // The following commands will talk about our 'vboID' buffer
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            // Give our vertices to OpenGL.
            glBufferData(GL_ARRAY_BUFFER, test.numFloats, test.vertexBufferData, GL_DYNAMIC_DRAW);
            
            currentShader = Shader::initShader("./assets/shaders/default.glsl");
            if (currentShader.filepath == nullptr) exit(-1);
            
            // TODO(HilbertCurve): automate the glTF file reading process here.
            
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
                                  GL_TRUE,
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
            
            glDrawArrays(GL_TRIANGLES, 0, sizeof(test.vertexBufferData)); // Starting from vertex 0; 12 vertices total -> 4 triangles
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindVertexArray(0);
            Shader::detachShader(currentShader);
        }
    }
}