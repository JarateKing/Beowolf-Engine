#version 150

in vec2 TexCoords;
out vec4 PixelColor;

uniform sampler2D postProcessingTex; //Sharp Image
uniform sampler2D boxBlurTex; //Blurred Image
uniform sampler2D depthTexture;

const float near_plane = 0.1;
const float far_plane = 1000.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
	float minDistance = 0.0;
	float maxDistance = 1.0;

	vec4 focusColor = texture(postProcessingTex, TexCoords);
	vec4 outOfColor = texture(boxBlurTex, TexCoords);
	float dist = LinearizeDepth(texture(depthTexture, TexCoords).x) / (far_plane / 10);
	float cursorDist = LinearizeDepth(texture(depthTexture, vec2(0.5, 0.5)).x) / (far_plane / 10);
	float blur = smoothstep(minDistance, maxDistance, abs(dist - cursorDist)/cursorDist);
	
	PixelColor = mix(focusColor, outOfColor, blur);
}