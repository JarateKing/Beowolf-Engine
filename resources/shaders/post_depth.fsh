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
	
	
	
	if(!(depthValue >= 0.9) && ((distance - depthValue) <= 0.01))
	{
		tempColor = tempColor * 1.5;
		
		bool edge = false;
		vec2 texelSize = 1.0 / textureSize(depthMap, 0);
		
		for(int x = -2; x <= 2; ++x)
		{
			for(int y = -2; y <= 2; ++y)
			{
				if(!edge)
				{
					float pcfDepth = LinearizeDepth(texture(depthMap, TexCoords.xy + vec2(x, y) * texelSize).r) / (far_plane / 10);
					edge = (pcfDepth < 0.01 || pcfDepth > 0.99) ? true : false;
				}
			}
		}
		if(edge)
		{
			tempColor = vec4 (0.0, 0.0, 0.0, 1.0);
		}
	}
	
	PixelColor = tempColor;
}