#version 330 core

layout (location=0) in vec3 aPos;

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 fPos;

void main()
{
    fPos = uProjection * uView * vec4(aPos, 1.0);
    gl_Position = fPos;
}

