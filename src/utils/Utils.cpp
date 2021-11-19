#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Camera.h"

namespace Pontilus
{
    void loadFile(const char *filepath, char *data)
    {
        FILE *ptr = fopen(filepath, "rb");
        long filesize = 0;

        if (ptr == nullptr)
        {
            fprintf(stderr, "Could not open \"%s\".\n", filepath);
            exit(-1);
        }

        fseek(ptr, 0L, SEEK_END);
        filesize = ftell(ptr);
        rewind(ptr);

        data = new char[filesize];

        fread((void *)data, filesize, 1, ptr);
    }

    // important, this only works for 2D functions.
    glm::vec2 screenToWorldCoords(glm::vec2 screenPos)
    {
        // screen to gl_Position coords
        screenPos += glm::vec2(-256, -256);
        screenPos /= 256;

        // gl_Position coords to world coords
        screenPos.x *= Renderer::Camera::projectionWidth / 2;
        screenPos.y *= Renderer::Camera::projectionHeight / 2;

        return screenPos;
    }
}