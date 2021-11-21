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
uniform vec2 uCameraPos;

uniform vec2 uResolution;

out vec4 color;

vec4 blendColor(vec4 v1, vec4 v2)
{
    vec3 retColor = v1.rgb * v1.a + v2.rgb * (1.0 - v1.a) * v2.a;
    float retAlpha = v1.a + (1.0 - v1.a) * v2.a;
    return vec4(retColor, retAlpha);
}

float calculateLighting(float d, float intensity) 
{
	return 1.0 / (1.0 + (0.001 / intensity) * d);
}

void main()
{
    vec2 st = (fPos.xy);
    vec4 bColor = vec4(0.0);
    float alpha = 0.0;

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        vec2 lightPos = uLightPos[i].xy; // TODO: implement 3D lights

        /* Point light of radius 0.5 in game coords.

        if (distance(lightPos, st) < 0.5)
        {
            bColor = vec4(1.0);
        }
        */

        // distance between the current pixel and the light position
		float dist = distance(uLightPos[i].xy, st);
		// calculate brightness using the attenuation function
		float attenuation = calculateLighting(dist, uLightIntensity[i] / 1000);
		// accumulate the value into total lighting by adding
		bColor += uLightColor[i] * attenuation;
    }

    color = bColor;
}
