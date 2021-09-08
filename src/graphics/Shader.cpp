#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <GLES3/gl32.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "Shader.h"
//TODO: safety functions: Application::init() must've been called before we do ANYTHING with shaders.

namespace Application
{
    namespace Renderer
    {
        Shader initShader(const char* filepath) 
        {
            std::string shaderCode;
            std::ifstream shaderStream;
            shaderStream.open(filepath);
            
            if(shaderStream.is_open()){
                std::stringstream sstr;
                std::string linebuf;
                while (getline(shaderStream, linebuf)) 
                {
                    sstr << linebuf << "\n";
                }
                shaderCode = sstr.str();
                shaderStream.close();
            }else{
                printf("Could not open %s. Are you in the right directory?\n", filepath);
                getchar();
                return {};
            }
            
            Shader shader;
            shader.vertexID = glCreateShader(GL_VERTEX_SHADER);
            shader.fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
            
            std::string* splitShader = new std::string[3];
            
            std::regex rgx("(#type)( )+([a-zA-Z])+");
            std::sregex_token_iterator iter(shaderCode.begin(),
                                            shaderCode.end(),
                                            rgx,
                                            -1);
            std::sregex_token_iterator end;
            for (int i = 0; iter != end; ++iter) 
            {
                splitShader[i] = *iter;
                i++;
                if (i >= 3) {
                    break;
                }
            }
            
            shader.vertexSource = splitShader[1].c_str();
            shader.fragmentSource = splitShader[2].c_str();
            
            shader.filepath = filepath;
            
            GLint result = GL_FALSE;
            int infoLogLength;
            
            // Compile Vertex Shader
            printf("Compiling vertex shader: %s\n", shader.filepath);
            glShaderSource(shader.vertexID, 1, &shader.vertexSource , NULL);
            glCompileShader(shader.vertexID);
            
            // Check Vertex Shader
            glGetShaderiv(shader.vertexID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(shader.vertexID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if ( infoLogLength > 0 )
            {
                std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
                glGetShaderInfoLog(shader.vertexID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
                printf("%s\n", &vertexShaderErrorMessage[0]);
            }
            
            // compile fragment shader
            printf("Compiling fragment shader: %s\n", shader.filepath);
            glShaderSource(shader.fragmentID, 1, &shader.fragmentSource, NULL);
            glCompileShader(shader.fragmentID);
            
            // check fragment shadieur
            glGetShaderiv(shader.fragmentID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(shader.fragmentID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
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
                std::vector<char> shaderLinkingErrorMessage(infoLogLength+1);
                glGetShaderInfoLog(shader.shaderProgramID, infoLogLength, NULL, &shaderLinkingErrorMessage[0]);
                printf("%s\n", &shaderLinkingErrorMessage[0]);
            }
            
            return shader;
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
        
        void Shader::clear()
        {
            // TODO: implement deletion
            
            glDeleteShader(this->vertexID);
            glDeleteShader(this->fragmentID);
        }
        
        void Shader::uploadMat4(const char* name, const glm::mat4& data)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed) this->attach();
            
            glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(data));
        }
        
        void Shader::uploadFloat(const char* name, const float& data)
        {
            GLint varLocation = glGetUniformLocation(this->shaderProgramID, name);
            if (!this->beingUsed) this->attach();
            
            glUniform1fv(varLocation, 1, &data);
        }
    }
}