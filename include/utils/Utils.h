/* date = October 17th 2021 5:25 pm */

#ifndef _PONTILUS_UTILS_H
#define _PONTILUS_UTILS_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/gl.h>

#define __alCall(fun, ...) \
{\
    fun(__VA_ARGS__);\
    if (int err = alGetError())\
    {\
        __pError("OpenAL error code 0x%04X recieved.", err);\
    }\
}

#define __pError(message, ...) \
{\
    fprintf(stderr, "\x1B[01m\x1B[31mERROR: Error at file %s, line %d:\x1B[0m ",\
            __FILE__, __LINE__);\
    fprintf(stderr, message __VA_OPT__(,) __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(-1);\
}

#define __pWarning(message, ...) \
{\
    fprintf(stdout, "\x1B[01m\x1B[31mWARNING: Warning at file %s, line %d:\x1B[0m ",\
            __FILE__, __LINE__);\
    fprintf(stdout, message __VA_OPT__(,) __VA_ARGS__);\
    fprintf(stdout, "\n");\
}

#define __pMessage(message, ...) \
{\
    printf("\x1B[01m\x1B[32mINFO: Message at file %s, line %d:\x1B[0m ",\
            __FILE__, __LINE__);\
    printf(message __VA_OPT__(,) __VA_ARGS__);\
    printf("\n");\
}

#define __pAssert(x, reason, ...) \
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

    glm::vec3 screenToWorldCoords(const glm::vec2 screenPos);
    glm::vec3 screenToWorldSize(const glm::vec2 screenSize);
}

#endif // _PONTILUS_UTILS_H