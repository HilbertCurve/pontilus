#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec3 aNormal;

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 fPos;
out vec4 fColor;
out vec3 fNormal;

void main()
{
    fPos = uProjection * uView * vec4(aPos, 1.0);
    fColor = aColor;
    fNormal = aNormal;
    gl_Position = fPos;
}

