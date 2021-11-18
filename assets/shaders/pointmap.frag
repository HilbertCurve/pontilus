#version 430 core
#ifdef GL_ES
 precision mediump float;
#endif

in vec3 fPos;
in vec4 fColor;

#define MAX_LIGHTS 4

uniform vec3 uLightPos[MAX_LIGHTS];
uniform vec4 uLightColor[MAX_LIGHTS];
uniform float uLightIntensity[MAX_LIGHTS];

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 color;

void main()
{
    if (fPos.x > 16)
    {
        color = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }

    vec2 st = fPos.xy;
    vec4 bColor = vec4(0.0);

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        vec4 lightPos = uProjection * uView * vec4(uLightPos[i], 1.0);


        bColor += (uLightColor[i] - vec4(vec3(distance(st, lightPos.xy)), 0.0)) * uLightIntensity[i];
    }

	color = bColor;
    
}
