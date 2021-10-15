/* date = August 28th 2021 5:09 pm */

#ifndef _PONTILUS_SHADER_H
#define _PONTILUS_SHADER_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pontilus
{
    namespace Renderer
    {
        namespace Shader
        {
            struct Shader
            {
                GLuint vertexID;
                GLuint fragmentID;
                const char *vertexSource;
                const char *fragmentSource;

                const char *filepath;
                GLuint shaderProgramID;

                bool beingUsed;
            };

            void uploadMat4(Shader &s, const char *name, const glm::mat4 &data);
            void uploadFloat(Shader &s, const char *name, const float &data);
            void uploadIntArr(Shader &s, const char *name, const int *data, int count);

            Shader initShader(const char *filepath);
            void initShader(const char *filepath, Shader &dest);

            void attachShader(Shader &s);
            void detachShader(Shader &s);
            void deleteShader(Shader &s);
        }
    }
}

#endif //_PONTILUS_SHADER_H
