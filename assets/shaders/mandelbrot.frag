#version 330 core

in vec4 fPos;
in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform float uTime;

out vec4 color;

// constants
const float PI = 3.14159265359;
const float epsilon = 4;
float iters = 20;//clamp(uTime * 3, 1.0, 50.0);
const vec2 zVal = vec2(0.0, 0.0); // use vec2(0.0) for mandelbrot set
const vec3 fadeColor = vec3(0.2, 0.4, 0.8);
const vec2 xyOffset = vec2(0.0);
const vec2 xyScale = vec2(2.0, 2.0);

// imaginary number tools

/**
 * Treat inputs as imaginary points, and multiply them together
 */
vec2 imagMul(vec2 p1, vec2 p2)
{
    float real = p1.x*p2.x - p1.y*p2.y;
    float imag = p1.x*p2.y + p1.y*p2.x;
    
    return vec2(real, imag);
}

/**
 * Find the distance between two points
 */
float dist(vec2 p1, vec2 p2)
{
    float xdiff = p1.x - p2.x;
    float ydiff = p1.y - p2.y;
    
    return sqrt(xdiff*xdiff + ydiff*ydiff);
}

// function that asymptotes towards 0.3, for fading
float fade(float x)
{
    return (1 / (x*x)) + 0.5;
}

vec2 iterVal(vec2 point)
{
    color = vec4(0.0);
    vec2 ret = imagMul(zVal, zVal) + point;
    for (int i = 0; i < iters; i++)
    {
        ret = imagMul(ret, ret) + point;
        if (dist(ret, vec2(0.0)) > 4.0)
        {
            color = vec4(vec3(fade(float(i / 5) + 1)) * fadeColor, 1.0);
            return ret;
        }
    }

    return ret;
}

void main()
{
    vec2 pos = fPos.xy * xyScale + xyOffset;

    float val = clamp(dist(iterVal(pos), vec2(0.0)), 0.0, 100.0);
    if (color != vec4(0.0))
    {
        return;
    }

    color = vec4(vec3(0.0), 1.0);
}

