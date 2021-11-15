#version 430 core
#ifdef GL_ES
 precision mediump float;
#endif

in vec3 fPos;
in vec4 fColor;

struct pointLight
{
    vec3 pos;
    vec4 color;
    float intensity;
};

// pos pos pos color color color color intensity

uniform float uLight[16][8];
uniform mat4 uProjection;
uniform mat4 uView;

out vec4 color;

void main()
{
    if (fPos.x > 16)
    {
        color = vec4(1.0, 0.0, 1.0, 0.0);
        return;
    }
    vec2 st = fPos.xy;
    float pct = 0.0;

    for (int i = 0; i < 4; i++)
    {
        vec4 lightPos = uProjection * uView * vec4(uLight[i][0], uLight[i][1], uLight[i][2], 1.0);

        pct += distance(st, lightPos.xy);
    }

    vec3 bColor = vec3(1.0) - vec3(pct);

	color = vec4( bColor/3.0, 0.1 );
    
}
