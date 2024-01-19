/* date = August 28th 2021 5:09 pm */

#pragma once

#include <vector>

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace Pontilus
{
    namespace Renderer
    {

        class Shader
        {
            GLuint vertexID;
            GLuint fragmentID;

            GLuint shaderProgramID;

            bool beingUsed;

            public:
            GLuint id() { return shaderProgramID; }

            void uploadMat4(const char *name, const glm::mat4 &data);
            void uploadFloat(const char *name, const float &data);
            void uploadInt(const char *name, const int data);
            void uploadIntArr(const char *name, const int *data, int count);
            void uploadFloatArr(const char *name, float *arr, int count);
            void uploadVec2Arr(const char *name, float *arr, int count);
            void uploadVec3Arr(const char *name, float *arr, int count);
            void uploadVec4Arr(const char *name, float *arr, int count);

            void attach();
            void detach();

            Shader() = default;
            Shader(std::string vertPath, std::string fragPath);
            ~Shader();

            // TODO: include layout information
        };

        void setDefaultShader(const char *vertPath, const char *fragPath);
        const char *getDefaultShader(bool oneForVert);
    }
}
