#version 430 core
#ifdef GL_ES
 precision mediump float;
#endif

in vec2 fPos;
in vec4 fColor;

struct pointLight
{
    vec3 pos;
    vec4 color;
    float intensity;
};

// pos pos pos color color color color intensity

uniform float uLight[16][8];

out vec4 color;

void main()
{
    vec2 st = fPos;

    float pct = 0.0;

    // a. The DISTANCE from the pixel to the center
    pct = distance(st,vec2(0.5));

    // b. The LENGTH of the vector
    //    from the pixel to the center
    // vec2 toCenter = vec2(0.5)-st;
    // pct = length(toCenter);

    // c. The SQUARE ROOT of the vector
    //    from the pixel to the center
    // vec2 tC = vec2(0.5)-st;
    // pct = sqrt(tC.x*tC.x+tC.y*tC.y);

    vec3 sColor = vec3(pct);

	color = vec4( sColor, 1.0 );
    
}
