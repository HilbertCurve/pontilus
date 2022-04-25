#include "utils/Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "graphics/Camera.h"
#include "core/Application.h"

namespace Pontilus
{
    void loadFile(const char *filepath, File &f, bool isBinary)
    {
        f.isBinary = isBinary;

        FILE *file;

        if (isBinary)
        {
            file = fopen(filepath, "rb");
        }
        else
        {
            file = fopen(filepath, "r");
        }

        fseek(file, 0, SEEK_END);
        f.size = ftell(file);
        fseek(file, 0, SEEK_SET);

        f.buffer = malloc(f.size);

        fread(f.buffer, f.size, (size_t)(sizeof(char)), file);

        fclose(file);
    }

    void freeFile(File &f)
    {
        free(f.buffer);
        f.size = 0;
    }

    using json = nlohmann::json;
    json getJSON(const char *filepath)
    {
        FILE *f = fopen(filepath, "r");

        fseek(f, 0, SEEK_END);
        int fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *strbuff = (char *)malloc(fsize + 1);
        fread(strbuff, fsize, sizeof(char), f);
        strbuff[fsize] = '\0';

        std::string src = std::string(strbuff);
        json j = json::parse(src.begin(), src.end());

        free(strbuff);
        fclose(f);

        return j;
    }

    using namespace glm;
    vec3 screenToWorldCoords(const vec2 screenPos)
    {
        vec4 v = {screenPos, 0.0f, 1.0f};

        v -= vec4{window.width / 2, window.height / 2, 0.0f, 0.0f};
        v /= vec4{window.width / 2, -window.height / 2, 1.0f, 1.0f};
        
        v = inverse(Renderer::Camera::getView()) * inverse(Renderer::Camera::getProjection()) * v;

        return {v.x, v.y, v.z};
    }

    vec3 screenToWorldSize(const vec2 screenSize)
    {
        vec4 v = {screenSize, 0.0f, 1.0f};
        // pretend that 0, 0 in screen space is 0, 0 in world space

        v /= vec4{window.width / 2, -window.height / 2, 1.0f, 1.0f};

        v = inverse(Renderer::Camera::getView()) * inverse(Renderer::Camera::getProjection()) * v;

        return {v.x, v.y, 0.0f};
    }

    bool debugMode()
    {
        return (*getArgs() & 0x0001) == 1;
    }

    bool echoOn()
    {
        return (*getArgs() & 0x0002) == 1;
    }

    void setEcho(bool on)
    {
        *getArgs() ^= on << 1;
    }
}
