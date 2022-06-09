#version 330 core

in vec4 fPos;
in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform float uTime;

out vec4 color;

// constants
const float PI = 3.14159265359;
const float E = 2.7182818284;
const float scale = 4.0;
float zoom = uTime;
const float magScale = 2.0;
const float magMod = 1.0;
bool doMod = true;

// animation
float timeMod = 0.0;

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

/**
 * Convert from cartesian coords to polar coords
 */
vec2 toArgMag(vec2 point)
{
    float arg = atan(point.y / point.x) / PI;
    float mag = dist(point, vec2(0.0, 0.0));
    
    // hacky way to do modulating
    if (point.x < 0.0)
    {
        arg += 1.0;
    }
    else if (point.y < 0.0)
    {
        arg += 2.0;
    }
    
    return vec2(arg / 2.0, mag);
}

/**
 * Convert from polar coords to cartesian coords
 */
vec2 toCartesian(vec2 point)
{
    return vec2(cos(point.x), sin(point.x)) * point.y;
}

/**
 * Calculate the value of a complex number raised to some power
 */
vec2 imagPow(vec2 base, vec2 xp)
{
    // FIXME: i don't know where the issue lies atm

    // get polar of base
    vec2 polar = toArgMag(base); // x, y => theta, r
    // create new polar point
    float lnVal = log(polar.y);

    float arg = lnVal * xp.y;
    vec2 newPolar = vec2(
            lnVal * xp.y + xp.x * polar.x,
            lnVal * xp.x - xp.y * polar.x
            );

    return toCartesian(newPolar);
}

bool between(float left, float x, float right)
{
    return x >= left && x < right;
}

// input function; changes over time
vec2 func(vec2 point)
{
    float time = uTime - timeMod;
    // mild hsv wheel
    if (between(0.0, time, 5.0)) {
        doMod = false;
        zoom = 0.5;
        return point;
    } else if (between(5.0, time, 10.0)) {
        // cool thingy
        doMod = true;
        zoom = 1.0;
        return imagMul(point, point) - vec2(sin(uTime) * 2, 0.0);
    } else if (between(10.0, time, 15.0)) {
        // kinda looks like braids
        doMod = false;
        zoom = uTime;
        return imagMul(sin(point), cos(point));
    } else if (between(15.0, time, 20.0)) {
        // ukranian flag-ish; may the war end soon
        doMod = false;
        zoom = 4.0;
        if (point.y > 0)
        {
            return toCartesian(vec2(PI / 4 + 0.15, 1.0));
        }
        else
        {
            return vec2(-1.0, -1.0);
        }
    } else {
        timeMod += 20.0;
        return vec2(0.0);
    }

    // broken
    //return imagPow(point, vec2(2.0, 0.0));
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

float scaleMag(float mag)
{
    return log(mag - 0.5) + 1.0;
}

void main()
{
    // find input position
    vec2 pos = (fPos.xy) * zoom;

    // calculate output based on complex function
    // then convert it to polar coords
    vec2 val = toArgMag(func(pos * scale));

    // clamp and rescale magnitude, for visualization
    if (doMod)
        val = vec2(val.x, mod(val.y / (magScale * magMod), magMod));
    else
        val = vec2(val.x, clamp(val.y, 0.0, 1.0));

    // set this as our color
    vec3 col = vec3(val.x, val.y, val.y);

    // find that color in rgb
    vec3 newCol = hsv2rgb(col);

    // Output to screen
    color = vec4(newCol,1.0);
}

