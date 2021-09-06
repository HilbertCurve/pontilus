#include <math.h>
#include "Math.h"

namespace Application
{
    namespace Math
    {
        //TODO(HilbertCurve): more math
        
        Vec2 Vec2::operator+(const Vec2 other) { return Vec2{x+other.x, y+other.y}; }
        Vec2 Vec2::operator-(const Vec2 other) { return Vec2{x-other.x, y-other.y}; }
        
        Vec3 Vec3::operator+(const Vec3 other) { return Vec3{x+other.x, y+other.y, z+other.z}; }
        Vec3 Vec3::operator-(const Vec3 other) { return Vec3{x-other.x, y-other.y, z-other.z}; }
        
        Vec4 Vec4::operator+(const Vec4 other) { return Vec4{x+other.x, y+other.y, z+other.z, w+other.w}; }
        Vec4 Vec4::operator-(const Vec4 other) { return Vec4{x-other.x, y-other.y, z-other.z, w-other.w}; }
        
        float Vec4::dot(Vec4 v)
        {
            return x*v.x + y*v.y + z*v.z + w*v.w;
        }
        
        Mat4 Mat4::operator+(const Mat4 other)
        {
            return Mat4 
            { 
                m00+other.m00, m01+other.m01, m02+other.m02, m03+other.m03,
                m10+other.m10, m11+other.m11, m12+other.m12, m13+other.m13,
                m20+other.m20, m21+other.m21, m22+other.m22, m23+other.m23,
                m30+other.m30, m31+other.m31, m32+other.m32, m33+other.m33
            };
        }
        
        Mat4 Mat4::operator-(const Mat4 other)
        {
            return Mat4 
            { 
                m00-other.m00, m01-other.m01, m02-other.m02, m03-other.m03,
                m10-other.m10, m11-other.m11, m12-other.m12, m13-other.m13,
                m20-other.m20, m21-other.m21, m22-other.m22, m23-other.m23,
                m30-other.m30, m31-other.m31, m32-other.m32, m33-other.m33
            };
        }
        
        Mat4 Mat4::operator*(const Mat4 m)
        {
            return Mat4
            {
                m00*m.m00 + m01*m.m10 + m02*m.m20 + m03*m.m30,
                m10*m.m00 + m11*m.m10 + m12*m.m20 + m13*m.m30,
                m20*m.m00 + m21*m.m10 + m22*m.m20 + m23*m.m30,
                m30*m.m00 + m31*m.m10 + m32*m.m20 + m33*m.m30,
                
                m00*m.m01 + m01*m.m11 + m02*m.m21 + m03*m.m31,
                m10*m.m01 + m11*m.m11 + m12*m.m21 + m13*m.m31,
                m20*m.m01 + m21*m.m11 + m22*m.m21 + m23*m.m31,
                m30*m.m01 + m31*m.m11 + m32*m.m21 + m33*m.m31,
                
                m00*m.m02 + m01*m.m12 + m02*m.m22 + m03*m.m32,
                m10*m.m02 + m11*m.m12 + m12*m.m22 + m13*m.m32,
                m20*m.m02 + m21*m.m12 + m22*m.m22 + m23*m.m32,
                m30*m.m02 + m31*m.m12 + m32*m.m22 + m33*m.m32,
                
                m00*m.m03 + m01*m.m13 + m02*m.m23 + m03*m.m33,
                m10*m.m03 + m11*m.m13 + m12*m.m23 + m13*m.m33,
                m20*m.m03 + m21*m.m13 + m22*m.m23 + m23*m.m33,
                m30*m.m03 + m31*m.m13 + m32*m.m23 + m33*m.m33
            };
        }
        
        Vec4 Mat4::operator*(const Vec4 v)
        {
            return Vec4
            {
                m00*v.x + m01*v.y + m02*v.z + m03*v.w,
                m10*v.x + m11*v.y + m12*v.z + m13*v.w,
                m20*v.x + m21*v.y + m22*v.z + m23*v.w,
                m30*v.x + m31*v.y + m32*v.z + m33*v.w
            };
        }
        
        Mat4 Mat4::identity()
        {
            return Mat4
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }
        
        Mat4& Mat4::setLookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
        {
            *this = setLookAt(eye.x, eye.y, eye.z,
                              center.x, center.y, center.z,
                              up.x, up.y, up.z);
            
            return *this;
        }
        
        // BIG thanks to httpdigest here; it doesn't make sense to me, but it'll work
        Mat4& Mat4::setLookAt(float eyeX, float eyeY, float eyeZ,
                              float centerX, float centerY, float centerZ,
                              float upX, float upY, float upZ)
        {
            // Compute direction from position to lookAt
            float dirX, dirY, dirZ;
            dirX = eyeX - centerX;
            dirY = eyeY - centerY;
            dirZ = eyeZ - centerZ;
            // Normalize direction
            float invDirLength = invsqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
            dirX *= invDirLength;
            dirY *= invDirLength;
            dirZ *= invDirLength;
            // left = up x direction
            float leftX, leftY, leftZ;
            leftX = upY * dirZ - upZ * dirY;
            leftY = upZ * dirX - upX * dirZ;
            leftZ = upX * dirY - upY * dirX;
            // normalize left
            float invLeftLength = invsqrt(leftX * leftX + leftY * leftY + leftZ * leftZ);
            leftX *= invLeftLength;
            leftY *= invLeftLength;
            leftZ *= invLeftLength;
            // up = direction x left
            float upnX = dirY * leftZ - dirZ * leftY;
            float upnY = dirZ * leftX - dirX * leftZ;
            float upnZ = dirX * leftY - dirY * leftX;
            
            this->m00 = leftX;
            this->m01 = upnX;
            this->m02 = dirX;
            this->m03 = 0.0f;
            this->m10 = leftY;
            this->m11 = upnY;
            this->m12 = dirY;
            this->m13 = 0.0f;
            this->m20 = leftZ;
            this->m21 = upnZ;
            this->m22 = dirZ;
            this->m23 = 0.0f;
            this->m30 = -(leftX * eyeX + leftY * eyeY + leftZ * eyeZ);
            this->m31 = -(upnX * eyeX + upnY * eyeY + upnZ * eyeZ);
            this->m32 = -(dirX * eyeX + dirY * eyeY + dirZ * eyeZ);
            this->m33 = 1.0f;
            
            return *this;
        }
        
        Mat4& Mat4::setOrtho(float left, float right, float bottom, float top, float near, float far)
        {
            *this = Mat4::identity();
            
            this->m00 = 2 / (right - left);
            this->m11 = 2 / (top - bottom);
            this->m22 = 2 / (near - far);
            this->m30 = (right + left) / (left - right);
            this->m31 = (bottom + top) / (bottom - top);
            this->m32 = (near + far) / (near - far);
            
            return *this;
        }
        
        template<typename T> void intoArray(Vec2);
        template<typename T> void intoArray(Vec3);
        template<typename T> void intoArray(Vec4);
        template<typename T> void intoArray(Mat4); 
        
        float invsqrt(float f)
        {
            // I could do this in a better way, but not now.
            return 1.0 / sqrt(f);
        }
    }
}
