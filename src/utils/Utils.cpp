#include "utils/Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <vendor/json/include/nlohmann/json.hpp>

#include "renderer/Camera.h"
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

    bool fileExists(const char *filepath)
    {
        FILE *f = fopen(filepath, "r");

        bool ret = f; // if f == nullptr, ret = 0

        if (ret)
            fclose(f);

        return ret;
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

        Window &window = Application::get()->getMainWindow();

        v -= vec4{window.getWidth() / 2, window.getHeight() / 2, 0.0f, 0.0f};
        v /= vec4{window.getWidth() / 2, -window.getHeight() / 2, 1.0f, 1.0f};
        
        v = inverse(window.getCamera().getView()) * inverse(window.getCamera().getProjection()) * v;

        return {v.x, v.y, v.z};
    }

    vec3 screenToWorldSize(const vec2 screenSize)
    {
        vec4 v = {screenSize, 0.0f, 1.0f};

        Window &window = Application::get()->getMainWindow();
        // pretend that 0, 0 in screen space is 0, 0 in world space

        v /= vec4{window.getWidth() / 2, -window.getHeight() / 2, 1.0f, 1.0f};

        v = inverse(window.getCamera().getView()) * inverse(window.getCamera().getProjection()) * v;

        v -= vec4(window.getCamera().getPosition(), 0.0f);

        return {v.x, v.y, 0.0f};
    }
}
