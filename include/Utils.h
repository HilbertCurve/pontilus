/* date = October 17th 2021 5:25 pm */

#ifndef _PONTILUS_UTILS_H
#define _PONTILUS_UTILS_H

namespace Pontilus
{

    /**
     * Use this struct if you want to return a pair of useful data in a function.
     */
    //template<typename T.
    struct BiReturn
    {
        void *first;
        int second;
    };

    void loadFile(const char *filepath, char *data);
}

#endif // _PONTILUS_UTILS_H