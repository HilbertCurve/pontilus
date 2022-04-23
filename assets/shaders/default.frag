#version 330 core

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
    if (fTexID == 1.0)
    {
        color = fColor * texture(uTextures[0], fTexCoords);
    }
    else if (fTexID == 2.0)
    {
        color = fColor * texture(uTextures[1], fTexCoords);
    }
    else if (fTexID == 3.0)
    {
        color = fColor * texture(uTextures[2], fTexCoords);
    }
    else if (fTexID == 4.0)
    {
        color = fColor * texture(uTextures[3], fTexCoords);
    }
    else if (fTexID == 5.0)
    {
        color = fColor * texture(uTextures[4], fTexCoords);
    }
    else if (fTexID == 6.0)
    {
        color = fColor * texture(uTextures[5], fTexCoords);
    }
    else if (fTexID == 7.0)
    {
        color = fColor * texture(uTextures[6], fTexCoords);
    }
    else if (fTexID == 8.0)
    {
        color = fColor * texture(uTextures[7], fTexCoords);
    }
    else
    {
        color = fColor;
    }

/*
    if (fTexID > 0.0) 
    {
        int id = int(fTexID);
        color = fColor * texture(uTextures[id - 1], fTexCoords); // - vignetteConst;
    } 
    else
    {
        color = fColor; // - vignetteConst;
    }
*/
}
