#include <stdio.h>
#include "Math.h"

namespace Application
{
    namespace Math
    {
        float Dot(Vector4 a, Vector4 b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }
        
        Vector4 Add(Vector4 a, Vector4 b)
        {
            Vector4 result{};
            result.x = a.x + b.x;
            result.y = a.y + b.y;
            result.z = a.z + b.z;
            result.w = a.w + b.w;
            
            return result;
        }
        
        void Print(Vector4 v)
        {
            printf("Vector4: x = %2.3f, y = %2.3f, z = %2.3f, w = %2.3f\n", v.x, v.y, v.z, v.w);
        }
        
        void Zero(Vector4* v)
        {
            v->x = 0;
            v->y = 0;
            v->z = 0;
        }
        
        float Dot(Vector2 a, Vector2 b)
        {
            return a.x * b.x + a.y * b.y;
        }
        
        Vector2 Add(Vector2 a, Vector2 b) 
        {
            Vector2 result{};
            result.x = a.x + b.x;
            result.y = a.y + b.y;
            
            return result;
        }
        
        void Print(Vector2 v)
        {
            printf("Vector2: x = %2.3f, y = %2.3f\n", v.x, v.y);
        }
    }
}