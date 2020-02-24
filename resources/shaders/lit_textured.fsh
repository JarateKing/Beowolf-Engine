#version 150

uniform sampler2D tex;
uniform sampler2D shadowMap;

// Light
uniform vec4 LightAmbient;
uniform vec4 LightDiffuse;
uniform vec3 LightDir;

in vec4 FragPosLightSpace;
in vec2 v_uv1;
in vec3 v_normal;
in vec4 v_color;

out vec4 PixelColor;

void main()
{	
	vec3 n = normalize( v_normal );

	vec4 ambient = LightAmbient * 0.7;

	vec4 diffuse = (LightDiffuse * max(0.0,dot(n, -LightDir)));
	
	vec3 projCoords = FragPosLightSpace.xyz/FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = 0.005; //max(0.05 * (1.0 - dot(v_normal, LightDir)), 0.05);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	vec4 light = clamp(ambient + diffuse, 0, 1);
	
    PixelColor = texture(tex, v_uv1) * vec4((1.0 - shadow) * light.rgb, 1.0);
}
