#version 150

in vec4 v_color;
in vec2 v_uv1;

out vec4 PixelColor;

void main()
{
	PixelColor = v_color;
}
