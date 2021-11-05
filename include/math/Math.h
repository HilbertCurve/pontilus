/* date = November 4th 2021 10:37 am */

#pragma once

namespace Pontilus
{
    namespace Math
    {
        //////////////////
        // STRUCT DECLS //
        //////////////////
        struct Mat2
        {
            float m00, m01;
            float m10, m11;

            Mat2 operator+(Mat2);
            Mat2 operator-(Mat2);
            Mat2 operator*(Mat2);

            float operator[](Mat2);
        };
        struct Mat3
        {
            float m00, m01, m02;
            float m10, m11, m12;
            float m20, m21, m22;

            Mat3 operator+(Mat3);
            Mat3 operator-(Mat3);
            Mat3 operator*(Mat3);

            float operator[](Mat3);
        };
        struct Mat4
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;

            Mat4 operator+(Mat4);
            Mat4 operator-(Mat4);
            Mat4 operator*(Mat4);

            float operator[](Mat4);
        };

        struct Vec2
        {
            float x, y;

            Vec2 operator+(Vec2);
            Vec2 operator-(Vec2);
            float operator[](Vec2);

            Vec2 operator*(Mat2);
        };
        struct Vec3
        {
            float x, y, z;

            Vec3 operator+(Vec3);
            Vec3 operator-(Vec3);
            float operator[](Vec3);

            Vec3 operator*(Mat3);
        };
        struct Vec4
        {
            float x, y, z, w;

            Vec4 operator+(Vec4);
            Vec4 operator-(Vec4);
            float operator[](Vec4);

            Vec4 operator*(Mat4);
        };

        ////////////////////
        // FUNCTION DECLS //
        ////////////////////
        int sign(float a);
        int signOrZero(float a);

        float dot(Vec2 v1, Vec2 v2);
        float dot(Vec3 v1, Vec3 v2);
        float dot(Vec4 v1, Vec4 v2);

        Mat2 inverse(Mat2 m);
        Mat3 inverse(Mat3 m);
        Mat4 inverse(Mat4 m);

        float *ptr(Vec2 v);
        float *ptr(Vec3 v);
        float *ptr(Vec4 v);

        float *ptr(Mat2 m);
        float *ptr(Mat3 m);
        float *ptr(Mat4 m);
    }
}