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
        namespace Shaders
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
            };

            void uploadMat4(const char* name, const glm::mat4& data);
            void uploadFloat(const char* name, const float& data);
            
            Shader initShader(const char* filepath);
            void initShader(const char *filepath, Shader &dest);

            void attachShader(Shader &s);
            void detachShader(Shader &s);
            void deleteShader(Shader &s);
        }
    }
}

#endif //_PONTILUS_SHADER_H
