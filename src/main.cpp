#include <stdio.h>

#include "Application.h"
#include "Math.h"

using namespace Application::Math;

int main() 
{
    Application::init();
    Application::loop();
    
    //Mat4 m {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    //Mat4 id = Mat4::identity();
    //Vec4 v {2, 3, 4, 5};
    //Vec4 v1 {1, 1, 1, 1};
    
    //Vec4 product1 = m * v;
    //float dot = v.dot(v1);
    //Mat4 m1 = m + id;
    
    //float* arr = new float[4];
    //intoArray(v, arr);
    
    //printf("%2.3f\n", m1.m00);
    //printf("%2.3f\n", dot);
    //printf("Array Converter: %2.3f\n", arr[0]);
    
    return 0;
}
