#version 150

in vec2 TexCoords;
out vec4 PixelColor;

uniform sampler2D postProcessingTex; //Sharp Image
uniform float percent;

void main()
{
	PixelColor = texture(postProcessingTex, TexCoords);
}