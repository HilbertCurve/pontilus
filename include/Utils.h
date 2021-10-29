/* date = October 17th 2021 5:25 pm */

#ifndef _PONTILUS_UTILS_H
#define _PONTILUS_UTILS_H

#include <stdint.h>

namespace Pontilus
{
    typedef int8_t byte;

    /**
     * Use this struct if you want to return a pair of useful data in a function.
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