/* date = August 28th 2021 5:09 pm */

#pragma once

#include <vector>

#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pontilus
{
    namespace Graphics
    {

        struct Shader
        {
            GLuint vertexID;
            GLuint fragmentID;
            const char *vertexSource;
            const char *fragmentSource;
            const char *vertPath;
            const char *fragPath;

            GLuint shaderProgramID;

            bool beingUsed;

            // TODO: include layout information
        };

        Shader initShader(const char *vertPath, const char *fragPath);

        void uploadMat4(Shader &s, const char *name, const glm::mat4 &data);
        void uploadFloat(Shader &s, const char *name, const float &data);
        void uploadInt(Shader &s, const char *name, const int data);
        void uploadIntArr(Shader &s, const char *name, const int *data, int count);
        void uploadFloatArr(Shader &s, const char *name, float *arr, int count);
        void uploadVec2Arr(Shader &s, const char *name, float *arr, int count);
        void uploadVec3Arr(Shader &s, const char *name, float *arr, int count);
        void uploadVec4Arr(Shader &s, const char *name, float *arr, int count);

        void attachShader(Shader &s);
        void detachShader(Shader &s);
        void deleteShader(Shader &s);

    }
}
