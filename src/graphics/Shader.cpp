#include "graphics/Shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Application.h"
#include "utils/Utils.h"
// TODO: safety functions: Application::init() must've been called before we do ANYTHING with shaders.

namespace Pontilus
{
    namespace Renderer
    {
        static const char *defaultVert = "../assets/shaders/default.vert";
        static const char *defaultFrag = "../assets/shaders/default.frag";

        Shader initShader(const char *vertPath, const char *fragPath)
        {
            // read vertex shader source
            FILE *vertFile = fopen(vertPath, "rb");
            int vertFilesize = 0;

            if (vertFile == nullptr)
            {
                __pError("Could not open \"%s\".\n", vertPath);
                exit(-1);
            }

            fseek(vertFile, 0L, SEEK_END);
            vertFilesize = ftell(vertFile);
            rewind(vertFile);

            char vertCode[vertFilesize];

            fread((void *)vertCode, vertFilesize, 1, vertFile);

            fclose(vertFile);

            // read fragment shader source
            FILE *fragFile = fopen(fragPath, "rb");
            int fragFilesize = 0;

            if (fragFile == nullptr)
            {
                __pError("Could not open \"%s\".\n", fragPath);
                exit(-1);
            }

            fseek(fragFile, 0L, SEEK_END);
            fragFilesize = ftell(fragFile);
            rewind(fragFile);

            char fragCode[fragFilesize];

            fread((void *)fragCode, fragFilesize, 1, fragFile);

            fclose(fragFile);

            Shader shader;
            shader.vertexID = glCreateShader(GL_VERTEX_SHADER);
            shader.fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

            shader.vertexSource = vertCode;
            shader.fragmentSource = fragCode;

            shader.vertPath = vertPath;
            shader.fragPath = fragPath;

            GLint result = GL_FALSE;
            int infoLogLength;

            // Compile Vertex Shader
            if (debugMode())
            {
                printf("Compiling vertex shader: %s\n", shader.vertPath);
            }
            glShaderSource(shader.vertexID, 1, &shader.vertexSource, &vertFilesize);
            glCompileShader(shader.vertexID);

            // Check Vertex Shader
            glGetShaderiv(shader.vertexID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(shader.vertexID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
                glGetShaderInfoLog(shader.vertexID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
                printf("%s\n", &vertexShaderErrorMessage[0]);
            }

            // compile fragment shader
            if (debugMode())
            {
                printf("Compiling fragment shader: %s\n", shader.fragPath);
            }
            glShaderSource(shader.fragmentID, 1, &shader.fragmentSource, &fragFilesize);
            glCompileShader(shader.fragmentID);

            // check fragment shadieur
            glGetShaderiv(shader.fragmentID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(shader.fragmentID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
                glGetShaderInfoLog(shader.fragmentID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
                printf("%s\n", &fragmentShaderErrorMessage[0]);
            }

            // link to program
            shader.shaderProgramID = glCreateProgram();
            glAttachShader(shader.shaderProgramID, shader.vertexID);
            glAttachShader(shader.shaderProgramID, shader.fragmentID);
            glLinkProgram(shader.shaderProgramID);

            // check linking
            glGetShaderiv(shader.shaderProgramID, GL_LINK_STATUS, &result);
            glGetShaderiv(shader.shaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> shaderLinkingErrorMessage(infoLogLength + 1);
                glGetShaderInfoLog(shader.shaderProgramID, infoLogLength, NULL, &shaderLinkingErrorMessage[0]);
                printf("%s\n", &shaderLinkingErrorMessage[0]);
            }

            return shader;
        }

        void attachShader(Shader &s)
        {
            // link shader to program
            glUseProgram(s.shaderProgramID);
            s.beingUsed = true;
        }

        void detachShader(Shader &s)
        {
            // unlink shader from program
            glUseProgram(0);
            s.beingUsed = false;
        }

        void deleteShader(Shader &s)
        {
            // TODO: implement deletion

            glDeleteShader(s.vertexID);
            glDeleteShader(s.fragmentID);
        }

        void uploadMat4(Shader &s, const char *name, const glm::mat4 &data)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);

            glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(data));
        }

        void uploadFloat(Shader &s, const char *name, const float &data)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);

            glUniform1fv(varLocation, 1, &data);
        }
        
        void uploadInt(Shader &s, const char *name, const int data)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);

            glUniform1i(varLocation, data);
        }

        /**
         * Note: data is the array in question, count is the number of elements in the array to upload.
         *
         */
        void uploadIntArr(Shader &s, const char *name, const int *data, int count)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);

            glUniform1iv(varLocation, count, data);
        }

        void uploadFloatArr(Shader &s, const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);
            
            glUniform1fv(varLocation, count, arr);
        }

        void uploadVec2Arr(Shader &s, const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);
            
            glUniform2fv(varLocation, count, arr);
        }

        void uploadVec3Arr(Shader &s, const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);
            
            glUniform3fv(varLocation, count, arr);
        }

        void uploadVec4Arr(Shader &s, const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(s.shaderProgramID, name);
            if (!s.beingUsed)
                attachShader(s);
            
            glUniform4fv(varLocation, count, arr);
        }

        void setDefaultShader(const char *vertPath, const char *fragPath)
        {
            defaultVert = vertPath;
            defaultFrag = fragPath;
        }

        const char *getDefaultShader(bool oneForVert)
        {
            return oneForVert ? defaultVert : defaultFrag;
        }
    }
}
