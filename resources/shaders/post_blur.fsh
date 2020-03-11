#version 150

in vec2 TexCoords;
out vec4 PixelColor;

uniform sampler2D postProcessingTex; //Sharp Image

void main()
{	
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