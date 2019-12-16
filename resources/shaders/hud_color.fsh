#version 150

uniform vec4 color;

in vec2 v_uv1;

out vec4 PixelColor;

void main()
{
	PixelColor = color;
}
