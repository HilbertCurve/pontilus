#version 430 core

#ifdef GL_ES
 precision mediump float;
#endif

in vec4 fPos;
in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform sampler2D uTextures[8];
uniform float uTime;

out vec4 color;

float dist(vec3 a, vec3 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

void main()
{
    vec3 point = vec3(0.0, 0.0, 0.0);
    float vignetteConst = dist(point, fPos.xyz) + (sin(2*uTime) * sin(2*uTime) / 10.0);
        
    if (fTexID > 0.0) 
    {
        int id = int(fTexID);
        color = fColor * texture(uTextures[id - 1], fTexCoords); // - vignetteConst;
    } 
    else
    {
        color = fColor; // - vignetteConst;
    }
}
