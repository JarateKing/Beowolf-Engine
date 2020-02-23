#version 150

uniform sampler2D tex;
uniform mat3 WorldIT;

// Light
//uniform vec4 LightSpecular;
uniform vec4 LightAmbient;
uniform vec4 LightDiffuse;
uniform vec3 LightDir;

in vec2 v_uv1;
in vec3 v_normal;
in vec4 v_color;

out vec4 PixelColor;

void main()
{	
	vec3 n = normalize( WorldIT * v_normal );

	// Ambient
	vec4 ambient = LightAmbient * 0.7;

	// Diffuse
	vec4 diffuse = (LightDiffuse * max(0.0,dot(n, -LightDir)));
	
	vec4 light = clamp(diffuse + ambient, 0, 1);
    PixelColor = texture(tex, v_uv1) * vec4(light.rgb, 1.0);
}
