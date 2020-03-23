#version 150

in vec2 TexCoords;
out vec4 PixelColor;

uniform sampler2D postProcessingTex; //Sharp Image
uniform sampler2D boxBlurTex; //Blurred Image
uniform sampler2D depthTexture;
uniform sampler2D depthMap;

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
	
	vec4 tempColor = mix(focusColor, outOfColor, blur);
	float distance = LinearizeDepth(texture(depthTexture, TexCoords).x) / (far_plane / 10);
	float depthValue = LinearizeDepth(texture(depthMap, TexCoords).x) / (far_plane / 10);
	if(!(depthValue >= 0.99) && ((distance - depthValue) <= 0.001))
	{
		tempColor = tempColor * 1.5;
	}
	PixelColor = tempColor;
	
	
	// SECONDARY DEPTH
	//if(depthValue >= 0.99)
	//{
	//	color = vec4(1.0, 0.0, 0.0, 1.0);
	//}
	//else
	//{
	//	color = vec4(vec3(depthValue), 1.0);
	//}
	//PixelColor = color;//vec4(vec3(depthValue), 1.0); // orthographic
}