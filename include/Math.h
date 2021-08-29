/* date = August 22nd 2021 5:22 pm */

#ifndef _MATH_H
#define _MATH_H

namespace Application
{
    namespace Math
    {
        struct Vector2
        {
            float x;
            float y;
        };
        
        struct Vector4
        {
            float x;
            float y;
            float z;
            float w;
        };
        
        float Dot(Vector4 a, Vector4 b);
        Vector4 Add(Vector4 a, Vector4 b);
        void Print(Vector4 v);
        void Zero(Vector4* v);
        
        float Dot(Vector2 a, Vector2 b);
        Vector2 Add(Vector2 a, Vector2 b);
        void Print(Vector2 v);
    }
}

#endif //_MATH_H
