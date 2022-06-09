#version 330 core

in vec4 fPos;
in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform float uTime;

out vec4 color;

// constants
const float PI = 3.14159265359;
const float epsilon = 0.1;
const float iters = 100;
const vec2 zVal = vec2(0.0); // use vec2(0.0) for mandelbrot set
const vec3 fadeColor = vec3(0.3, 0.4, 0.9);
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

vec2 iterVal(vec2 point)
{
    vec2 ret = imagMul(zVal, zVal) + point;
    for (int i = 0; i < iters; i++)
    {
        ret = imagMul(ret, ret) + point;
    }

    return ret;
}

// thanks to http://github.com/983 for this
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec2 pos = fPos.xy * xyScale;

    float val = dist(iterVal(pos), vec2(0.0));

    vec3 col;
    if (val < epsilon)
    {
        col = vec3(0.0);
    }
    else
    {
        col = vec3(val) * fadeColor;
    }

    color = vec4(col, 1.0);
}

