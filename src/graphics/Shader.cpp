#include "Shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "Utils.h"
// TODO: safety functions: Application::init() must've been called before we do ANYTHING with shaders.

namespace Pontilus
{
    namespace Graphics
    {

        Shader initShader(const char *vertPath, const char *fragPath)
        {
            // read vertex shader source
            FILE *vertFile = fopen(vertPath, "rb");
            int vertFilesize = 0;

            if (vertFile == nullptr)
            {
                fprintf(stderr, "Could not open \"%s\".\n", vertPath);
                exit(-1);
            }

            fseek(vertFile, 0L, SEEK_END);
            vertFilesize = ftell(vertFile);
            rewind(vertFile);

            char vertCode[vertFilesize];

            fread((void *)vertCode, vertFilesize, 1, vertFile);

            if (debugMode())
            {
                for (int i = 0; i < vertFilesize; i++)
                {
                    printf("%c", vertCode[i]);
                }
            }

            // read fragment shader source
            FILE *fragFile = fopen(fragPath, "rb");
            int fragFilesize = 0;

            if (fragFile == nullptr)
            {
                __pError("Could not open \"%s\".\n");
                exit(-1);
            }

            fseek(fragFile, 0L, SEEK_END);
            fragFilesize = ftell(fragFile);
            rewind(fragFile);

            char fragCode[fragFilesize];

            fread((void *)fragCode, fragFilesize, 1, fragFile);

            if (debugMode())
            {
                for (int i = 0; i < fragFilesize; i++)
                {
                    printf("%c", fragCode[i]);
                }
            }

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
            printf("Compiling vertex shader: %s\n", shader.vertPath);
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
            printf("Compiling fragment shader: %s\n", shader.fragPath);
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

    }
}