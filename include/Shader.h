/* date = August 28th 2021 5:09 pm */

#ifndef _SHADER_H
#define _SHADER_H

namespace Application
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
        };
        
        Shader initShader(const char* filepath);
        void attachShader(Shader &shader);
        void detachShader(Shader &shader);
        void deleteShader(Shader &shader);
    }
}

#endif //_SHADER_H
