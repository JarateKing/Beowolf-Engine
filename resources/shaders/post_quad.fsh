#version 150

in vec2 TexCoords;
out vec4 PixelColor;

uniform sampler2D postProcessingTex;

void main()
{
	PixelColor = texture(postProcessingTex, TexCoords);
}