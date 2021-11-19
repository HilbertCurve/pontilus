/* date = October 17th 2021 5:25 pm */

#ifndef _PONTILUS_UTILS_H
#define _PONTILUS_UTILS_H

#include <stdint.h>
#include <string.h>
#include <glm/glm.hpp>

#define __pError(message, ...) \
{\
    fprintf(stderr, "\x1B[01m\x1B[31mERROR: Error at file %s, line %d:\x1B[0m ",\
            __FILE__, __LINE__);\
    fprintf(stderr, message __VA_OPT__(,) __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(-1);\
}

#define __pMessage(message, ...) \
{\
    printf("\x1B[01m\x1B[32mINFO: Message at file %s, line %d:\x1B[0m ",\
            __FILE__, __LINE__);\
    printf(message __VA_OPT__(,) __VA_ARGS__);\
    printf("\n");\
}

#define __pAssert(x, reason) \
if (!(x))\
{\
    fprintf(stderr, "\x1B[01m\x1B[31mERROR: Assertion failed at file %s, line %d:\x1B[0m %s\n",\
            __FILE__, __LINE__, reason);\
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

    void loadFile(const char *filepath, char *data);

    glm::vec2 screenToWorldCoords(glm::vec2 screenPos);
}

#endif // _PONTILUS_UTILS_H