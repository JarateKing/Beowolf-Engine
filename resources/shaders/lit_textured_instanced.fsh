#version 150

uniform sampler2D tex;
uniform sampler2D shadowMap;

// Light
uniform vec4 LightAmbient;
uniform vec4 LightDiffuse;
uniform vec3 LightDir;
uniform float minheight;
uniform float maxheight;

in vec4 FragPosLightSpace;
in vec2 v_uv1;
in vec3 v_normal;
in vec4 v_color;
in float v_height;

out vec4 PixelColor;

void main()
{	
	if ((v_height > maxheight) || (v_height < minheight))
		discard;

	vec3 n = normalize( v_normal );

	vec4 ambient = LightAmbient * 0.7;

	vec4 diffuse = (LightDiffuse * max(0.0,dot(n, -LightDir)));
	
	vec3 projCoords = FragPosLightSpace.xyz/FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = 0.001; //max(0.05 * (1.0 - dot(v_normal, LightDir)), 0.05);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias> pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	
	if(projCoords.z > 1.0)
		shadow = 0.0;
	
	vec4 light = clamp(ambient + diffuse, 0, 1);
	
    PixelColor = texture(tex, v_uv1) * vec4((clamp(((1.0 - shadow) + 0.5), 0, 1) * light.rgb), 1.0);
}
