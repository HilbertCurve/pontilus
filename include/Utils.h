/* date = October 17th 2021 5:25 pm */

#pragma once

//#ifndef _PONTILUS_UTILS_H
//#define _PONTILUS_UTILS_H

#include <stdint.h>
#include "Rend.h"
#include "GameObject.h"
#include "Renderer.h"

namespace Pontilus
{
    typedef int8_t byte;

    /**
     * Use this struct if you want to return a pair of useful data in a function.
     */
    template<typename T, typename U>
    struct BiReturn
    {
        T first;
        U second;
    };

    // by way of the GCC folks and whatever goes on there,
    void printRend(Graphics::Rend &r); // THIS IS A VARIABLE,
    void loadFile(const char *filepath, char *data); // WHILE THIS IS NOT?
}

//#endif // _PONTILUS_UTILS_H