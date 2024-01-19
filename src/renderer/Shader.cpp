#include "renderer/Shader.h"

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

        Shader::Shader(std::string vertPath, std::string fragPath)
        {
            // read vertex shader source
            FILE *vertFile = fopen(vertPath.c_str(), "rb");
            int vertFilesize = 0;

            if (vertFile == nullptr)
            {
                __pError("Could not open \"%s\".\n", vertPath.c_str());
                exit(-1);
            }

            fseek(vertFile, 0L, SEEK_END);
            vertFilesize = ftell(vertFile);
            rewind(vertFile);

            char vertCode[vertFilesize];

            fread((void *)vertCode, vertFilesize, 1, vertFile);

            fclose(vertFile);

            // read fragment shader source
            FILE *fragFile = fopen(fragPath.c_str(), "rb");
            int fragFilesize = 0;

            if (fragFile == nullptr)
            {
                __pError("Could not open \"%s\".\n", fragPath.c_str());
                exit(-1);
            }

            fseek(fragFile, 0L, SEEK_END);
            fragFilesize = ftell(fragFile);
            rewind(fragFile);

            char fragCode[fragFilesize];

            fread((void *)fragCode, fragFilesize, 1, fragFile);

            fclose(fragFile);

            this->vertexID = glCreateShader(GL_VERTEX_SHADER);
            this->fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

            GLint result = GL_FALSE;
            int infoLogLength;

            // Compile Vertex Shader
            if (debugMode())
            {
                printf("Compiling vertex shader: %s\n", vertPath.c_str());
            }
            const char *vertPtr = &vertCode[0];
            glShaderSource(this->vertexID, 1, &vertPtr, &vertFilesize);
            glCompileShader(this->vertexID);

            // Check Vertex Shader
            glGetShaderiv(this->vertexID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(this->vertexID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
                glGetShaderInfoLog(this->vertexID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
                printf("%s\n", &vertexShaderErrorMessage[0]);
            }

            // compile fragment shader
            if (debugMode())
            {
                printf("Compiling fragment shader: %s\n", fragPath.c_str());
            }
            const char *fragPtr = &fragCode[0];
            glShaderSource(this->fragmentID, 1, &fragPtr, &fragFilesize);
            glCompileShader(this->fragmentID);

            // check fragment shadieur
            glGetShaderiv(this->fragmentID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(this->fragmentID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
                glGetShaderInfoLog(this->fragmentID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
                printf("%s\n", &fragmentShaderErrorMessage[0]);
            }

            // link to program
            this->shaderProgramID = glCreateProgram();
            glAttachShader(this->shaderProgramID, this->vertexID);
            glAttachShader(this->shaderProgramID, this->fragmentID);
            glLinkProgram(this->shaderProgramID);

            // check linking
            glGetShaderiv(this->shaderProgramID, GL_LINK_STATUS, &result);
            glGetShaderiv(this->shaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> shaderLinkingErrorMessage(infoLogLength + 1);
                glGetShaderInfoLog(this->shaderProgramID, infoLogLength, NULL, &shaderLinkingErrorMessage[0]);
                printf("%s\n", &shaderLinkingErrorMessage[0]);
            }
        }

        void Shader::attach()
        {
            // link shader to program
            glUseProgram(this->shaderProgramID);
            this->beingUsed = true;
        }

        void Shader::detach()
        {
            // unlink shader from program
            glUseProgram(0);
            this->beingUsed = false;
        }

        Shader::~Shader()
        {
            // TODO: implement deletion??


            glDeleteShader(this->vertexID);
            glDeleteShader(this->fragmentID);
        }

        void Shader::uploadMat4(const char *name, const glm::mat4 &data)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();

            glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(data));
        }

        void Shader::uploadFloat(const char *name, const float &data)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();

            glUniform1fv(varLocation, 1, &data);
        }
        
        void Shader::uploadInt(const char *name, const int data)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();

            glUniform1i(varLocation, data);
        }

        /**
         * Note: data is the array in question, count is the number of elements in the array to upload.
         *
         */
        void Shader::uploadIntArr(const char *name, const int *data, int count)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();

            glUniform1iv(varLocation, count, data);
        }

        void Shader::uploadFloatArr(const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();
            
            glUniform1fv(varLocation, count, arr);
        }

        void Shader::uploadVec2Arr(const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();
            
            glUniform2fv(varLocation, count, arr);
        }

        void Shader::uploadVec3Arr(const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();
            
            glUniform3fv(varLocation, count, arr);
        }

        void Shader::uploadVec4Arr(const char *name, float *arr, int count)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed)
                this->attach();
            
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
