/* date = August 22nd 2021 10:47 am */

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <GLES3/gl3.h>

namespace Application 
{
    struct Window;
    
    void init();
    void loop();
    
    GLuint* getProgramID();
}

#endif //_APPLICATION_H
