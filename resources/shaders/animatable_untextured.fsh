#version 150

uniform vec3 modelColor;
uniform vec3 modelAdditive;
uniform vec3 modelFilter;
uniform mat3 WorldIT;
//uniform vec3 ViewDir;

// Light
uniform vec4 LightSpecular;
uniform vec4 LightAmbient;
uniform vec4 LightDiffuse;
uniform vec3 LightDir;

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
	
	PixelColor = vec4(((v_color.rgb * v_color.a) + (modelColor * (1 - v_color.a))) * modelFilter + modelAdditive, 1) * vec4(light.rgb, 1.0);
}
