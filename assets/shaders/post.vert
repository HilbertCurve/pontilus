#version 430 core
#ifdef GL_ES
 precision mediump float;
#endif

layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;

uniform mat4 uProjection;
uniform mat4 uView;

out vec3 fPos;
out vec4 fColor;

void main()
{
    fPos = aPos;
    fColor = aColor;
    gl_Position = uProjection * uView * vec4(aPos.xy, 0.0, 1.0);
}
