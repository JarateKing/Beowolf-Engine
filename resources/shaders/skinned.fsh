#version 150

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float fade;
uniform mat3 WorldIT;
uniform vec3 ViewDir;

// Light
uniform vec4 LightAmbient;
uniform vec4 LightSpecular;
uniform vec4 LightDiffuse;
uniform vec3 LightDir;

uniform vec3 PointOnePos;
uniform float PointOneRange;
uniform vec4 PointOneColor;

uniform vec3 PointTwoPos;
uniform float PointTwoRange;
uniform vec4 PointTwoColor;

in vec2 v_uv1;
in vec3 v_normal;
in vec4 v_pos;
out vec4 PixelColor;

void main()
{
	vec3 n = normalize( v_normal );
	vec3 oneDir = PointOnePos - v_pos.xyz;
	float oneDist = length(oneDir);
	vec3 twoDir = PointTwoPos - v_pos.xyz;
	float twoDist = length(twoDir);
	
	vec4 pointOne = vec4(0,0,0,0);
	vec4 pointTwo = vec4(0,0,0,0);
	if (oneDist < PointOneRange * PointOneRange)
	{
		pointOne = PointOneColor * max(0.0,dot(n, oneDir)) * (1.0 / oneDist);
	}
	if (twoDist < PointTwoRange * PointTwoRange)
	{
		pointTwo = PointTwoColor * max(0.0,dot(n, twoDir)) * (1.0 / twoDist);
	}

	// Ambient
	vec4 ambient = LightAmbient;

	// Diffuse
	vec4 diffuse = (LightDiffuse * max(0.0,dot(n, -LightDir)));

	// Specular
	float specFactor = pow(max(dot(reflect(LightDir,n), ViewDir), 0.0), 1.0);
	vec4 specular = (specFactor * texture(texture2, v_uv1) * LightSpecular);
    
	vec4 light = clamp(ambient + diffuse + specular + pointOne + pointTwo, 0, 1);

    PixelColor = texture(texture1, v_uv1) * vec4( light.rgb, 1.0);
}
