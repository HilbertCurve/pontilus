/* date = October 17th 2021 5:25 pm */

#ifndef PONTILUS_UTILS_H
#define PONTILUS_UTILS_H

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <glad/gl.h>

// TODO: refactor to be templated functions

#define _alCall(fun, ...) \
{\
    fun(__VA_ARGS__);\
    if (int err = alGetError())\
    {\
        _pError("OpenAL error code 0x%04X recieved.", err);\
    }\
}

#define _pError(message, ...) \
{\
    fprintf(stderr, "\x1B[01m\x1B[31mERROR:\x1B[0m ");\
    fprintf(stderr, message __VA_OPT__(,) __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(-1);\
}

#define _pWarning(message, ...) \
if (true/*echoOn()*/) {\
    fprintf(stdout, "\x1B[01m\x1B[31mWARN:\x1B[0m ");\
    fprintf(stdout, message __VA_OPT__(,) __VA_ARGS__);\
    fprintf(stdout, "\n");\
}

#define _pMessage(message, ...) \
if (true/*echoOn()*/) {\
    printf("\x1B[01m\x1B[32mINFO:\x1B[0m ");\
    printf(message __VA_OPT__(,) __VA_ARGS__);\
    printf("\n");\
}

#define _pAssert(x, reason, ...) \
if (!(x))\
{\
    fprintf(stderr, "\x1B[01m\x1B[31mERROR: Assertion failed at file %s, line %d:\x1B[0m ",\
            __FILE__, __LINE__);\
    printf(reason __VA_OPT__(,) __VA_ARGS__);\
    printf("\n");\
    exit(-1);\
}

namespace Pontilus
{   
    typedef int8_t byte;

    /**
     * Typedef it like this:
     * typedef Pair<(type one), (type two)> one_two
     */
    template <typename T, typename U>
    struct Pair
    {
        T first;
        U second;
    };

    struct File
    {
        unsigned int size;
        
        bool isBinary;
        void *buffer;
    };

    void loadFile(const char *filepath, File &fb, bool isBinary);
    void freeFile(File &f);
    bool fileExists(const char *filepath);

    nlohmann::json getJSON(const char *filepath);

    // move to some math directory, or to Camera
    glm::vec3 screenToWorldCoords(const glm::vec2 screenPos);
    glm::vec3 screenToWorldSize(const glm::vec2 screenSize);
}

#endif // PONTILUS_UTILS_H

