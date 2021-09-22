/* date = August 28th 2021 5:09 pm */

#ifndef _PONTILUS_SHADER_H
#define _PONTILUS_SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pontilus
{
    namespace Renderer
    {
        struct Shader
        {
            GLuint vertexID;
            GLuint fragmentID;
            const char* vertexSource;
            const char* fragmentSource;
            
            const char* filepath;
            GLuint shaderProgramID;
            
            bool beingUsed;
            
            void attach();
            void detach();
            void clear();
            
            void uploadMat4(const char* name, const glm::mat4& data);
            void uploadFloat(const char* name, const float& data);
        };
        
        Shader initShader(const char* filepath);
    }
}

#endif //_PONTILUS_SHADER_H
