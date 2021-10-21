/* date = August 22nd 2021 10:47 am */

#ifndef _PONTILUS_APPLICATION_H
#define _PONTILUS_APPLICATION_H

namespace Pontilus
{
    typedef short _PONTILUS_SETTINGS;
    struct Window;

    void init();
    void loop();

    _PONTILUS_SETTINGS *getArgs();
    bool debugMode();
}

#endif //_PONTILUS_APPLICATION_H
