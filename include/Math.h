/* date = August 22nd 2021 5:22 pm */

#ifndef _PONTILUS_MATH_H
#define _PONTILUS_MATH_H

#include <stdio.h>
#include <cstring>

namespace Application
{
    namespace Math
    {
        typedef struct Vec2
        {
            float x;
            float y;
            
            Vec2 operator+(Vec2 other);
            Vec2 operator-(Vec2 other);
        } Vec2;
        
        typedef struct Vec3
        {
            float x;
            float y;
            float z;
            
            Vec3 operator+(Vec3 other);
            Vec3 operator-(Vec3 other);
        } Vec3;
        
        typedef struct Vec4
        {
            float x;
            float y;
            float z;
            float w;
            
            Vec4 operator+(Vec4 other);
            Vec4 operator-(Vec4 other);
            
            float dot(Vec4 v);
        } Vec4;
        
        typedef struct Mat4
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
            
            Mat4 operator+(Mat4 other);
            Mat4 operator-(Mat4 other);
            Mat4 operator*(Mat4 m);
            Vec4 operator*(Vec4 v);
            
            static Mat4 identity();
            
            Mat4 setTo(Mat4 m);
            
            Mat4& setLookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
            Mat4& setLookAt(float eyeX, float eyeY, float eyeZ,
                            float centerX, float centerY, float centerZ,
                            float upX, float upY, float upZ);
            
            Mat4& setOrtho(float left, float right, float bottom, float top, float near, float far);
        } Mat4;
        
        // i don't like how templates have to be instantiated in header files...
        template<typename T>
            void intoArray(const T& a, float* arr)
        {
            int size = sizeof(a)/sizeof(float);
            
            float data[size];
            
            // get the pointer to a, then move the pointer a little bit to the right until we've 
            // reached the end of the data struct. 
            for (int i = 0; i < size; i++)
            {
                // it automatically does the `* sizeof(float)` for us.
                data[i] = *((float*)&a + i);
            }
            
            delete[] arr;
            arr = new float[size];
            memcpy(arr, data, size * sizeof(float));
        }
        
        float invsqrt(float f);
    }
}

#endif //_PONTILUS_MATH_H
