#type vertex
#version 430 core
#ifdef GL_ES
 precision mediump float;
#endif

layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoords;
layout (location=3) in float aTexID;

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 fColor;
out vec2 fTexCoords;
out float fTexID;

void main()
{
    fColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    fTexCoords = aTexCoords;
    fTexID = aTexID;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}

#type fragment
#version 430 core
#ifdef GL_ES
 precision mediump float;
#endif

in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform sampler2D uTexture;

out vec4 color;

void main()
{
    if (fTexID > 0) 
    {
        int id = int(fTexID);
        color = fColor * texture(uTexture, fTexCoords);
        //color = vec4(fTexCoords, 0, 1);
    } 
    else
    {
        color = fColor;
    }
}