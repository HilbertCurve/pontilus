#version 330 core
#ifdef GL_ES
 precision mediump float;
#endif

layout (location=0) in vec4 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoords;
layout (location=3) in float aTexID;

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 fPos;
out vec4 fColor;
out vec2 fTexCoords;
out float fTexID;

void main()
{
    fPos = uProjection * uView * aPos;
    fColor = aColor;
    fTexCoords = aTexCoords;
    fTexID = aTexID;
    gl_Position = uProjection * uView * aPos;
}
