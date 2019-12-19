#version 150

in vec2 v_uv1;
in vec4 v_color;
out vec4 PixelColor;

void main()
{
	PixelColor = v_color;
}
