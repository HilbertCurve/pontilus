/* date = October 17th 2021 5:25 pm */

#ifndef _PONTILUS_UTILS_H
#define _PONTILUS_UTILS_H

#include <stdint.h>
#include <string.h>

#define __pError(message, ...) \
{\
    fprintf(stderr, "\x1B[31mERROR: Error at file %s, line %d:\x1B[0m ",\
            __FILE__, __LINE__);\
    fprintf(stderr, message __VA_OPT__(,) __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(-1);\
}

#define __pAssert(x, reason) \
if (!(x))\
{\
    fprintf(stderr, "\x1B[31mERROR: Assertion failed at file %s, line %d:\x1B[0m %s\n",\
            __FILE__, __LINE__, reason);\
    exit(-1);\
}

namespace Pontilus
{
    typedef int8_t byte;

    /**
     * Use this struct if you want to return a pair of useful data in a function.
     * Typedef it like this:
     * typedef BiReturn<(type one), (type two)> one_two
     */
    template <typename T, typename U>
    struct BiReturn
    {
        T first;
        U second;
    };

    void loadFile(const char *filepath, char *data);
}

#endif // _PONTILUS_UTILS_H