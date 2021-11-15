#version 430 core
#ifdef GL_ES
 precision mediump float;
#endif

layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;

out vec3 fPos;
out vec4 fColor;

void main()
{
    fPos = aPos;
    fColor = aColor;
    gl_Position = vec4(aPos.xyz, 1.0);
}