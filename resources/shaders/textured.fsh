#version 150

uniform sampler2D texture1;
uniform float fade;

// lights
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
	
	vec4 light = clamp(pointOne + pointTwo, 0, 1);
    PixelColor = texture(texture1, v_uv1) * vec4(light.rgb, 1.0);
}
