#version 150

in vec2 TexCoords;
out vec4 PixelColor;

uniform sampler2D postProcessingTex; //Sharp Image
//uniform sampler2D boxBlurTex; //Blurred Image
//uniform sampler2D depthTexture;
uniform float percent;

void main()
{
	//============================================================================//
	//===============================GrayScale====================================//
	//============================================================================//

	//float minDistance = 1.0;
	//float maxDistance = 3.0;

	//vec4 focusColor = texture(postProcessingTex, TexCoords);
	//vec4 outOfColor = texture(boxBlueTex, TexCoords);
	//vec4 focusPoint = texture(positionTexture, mouseFocusPoint);
	
	//float blur = smoothstep(minDistance, maxDistance, abs(position.y - focusPoint.y);

	//vec4 c = texture(postProcessingTex, TexCoords);
	//float L = dot(c.rgb, vec3(0.22, 0.707, 0.071));
	//PixelColor.rgb = mix(c.rgb,vec3(L,L,L),percent);
	//PixelColor.a = c.a;
	
	//============================================================================//
	//=================================Blur=======================================//
	//============================================================================//
	
	vec2 texelSize = 1.0 / textureSize(postProcessingTex, 0);
	
	vec4 upperRow = texture(postProcessingTex, TexCoords + (-texelSize.x, texelSize.y))
			+ texture(postProcessingTex, TexCoords + (0, texelSize.y))
			+ texture(postProcessingTex, TexCoords + (texelSize.x, texelSize.y));
			
	vec4 centerRow = texture(postProcessingTex, TexCoords + (-texelSize.x, 0))
			+ texture(postProcessingTex, TexCoords)
			+ texture(postProcessingTex, TexCoords + (texelSize.x, 0));
			
	vec4 bottomRow = texture(postProcessingTex, TexCoords + (-texelSize.x, -texelSize.y))
			+ texture(postProcessingTex, TexCoords + (0, -texelSize.y))
			+ texture(postProcessingTex, TexCoords + (texelSize.x, -texelSize.y));
			
	PixelColor = (upperRow + centerRow + bottomRow) / 9;
}