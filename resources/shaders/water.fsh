#version 150

uniform sampler2D tex;
uniform sampler2D reflection;
uniform sampler2D normaltex;
uniform sampler2D refraction;
uniform sampler2D fogDistance;
uniform float screenX;
uniform float screenY;

in vec2 v_uv1;
in vec2 v_uv2;
in vec2 v_uv3;
in vec2 v_uv4;
in float v_depth;

out vec4 PixelColor;

const float near_plane = 0.1;
const float far_plane = 1000.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
	vec4 wave1 = texture(tex, v_uv1);
	vec4 wave2 = texture(tex, v_uv2);
	float magnitude = (wave1.r * wave2.r);
	magnitude = magnitude * magnitude * magnitude * 3.0;
	
	vec4 offset = texture(normaltex, v_uv3);
	vec4 offset2 = texture(normaltex, v_uv4);
	
	vec2 reflectCoord = vec2(gl_FragCoord.x / screenX + (offset.r - 0.5) * 0.015, 1 - gl_FragCoord.y / screenY + (offset.g - 0.5) * 0.015);
	vec4 reflect = texture(reflection, reflectCoord);
	
	vec2 refractCoord = vec2(gl_FragCoord.x / screenX + (offset2.r - 0.5) * 0.02, gl_FragCoord.y / screenY + (offset.g - 0.5) * 0.02);
	vec4 refract = texture(refraction, refractCoord);
	
	float fogdist = LinearizeDepth(texture(fogDistance, refractCoord).x) / (far_plane / 10);
	float ref = v_depth / (far_plane / 10);
	float fog = (fogdist - ref);
	fog = fog * 16;
	fog = clamp(fog, 0, 1);
	
	float shoreAmount = fog * 10.0 + offset.r / 10.0;
	shoreAmount = 1 - clamp(shoreAmount, 0, 1);
	
	float shoreAmountVariance = fog * 2.0 + offset.g / 16.0;
	shoreAmountVariance = clamp(1 - shoreAmountVariance, 0, 1) * (wave1.r + wave2.r) * (wave1.r + wave2.r);
	
	shoreAmount = shoreAmount * 0.215 + shoreAmountVariance * 0.175;
	
	vec4 baseColor = mix(mix(refract, vec4(0.1, 0.2, 0.5, 1.0), fog), reflect, 0.25);
	
	PixelColor = vec4(-0.1, 0.1, 0.15, 0) +
				 vec4(shoreAmount, shoreAmount, shoreAmount, 0) + 
				 baseColor + 
				 vec4(magnitude, magnitude, magnitude, magnitude);
}
