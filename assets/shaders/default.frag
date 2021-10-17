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