#version 330 core

in vec4 fPos;
in vec4 fColor;
in vec3 fNormal;

out vec4 color;

void main()
{
    color = fColor * vec4(fNormal, 1.0);
}

