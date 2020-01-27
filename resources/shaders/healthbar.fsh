#version 150

uniform float threshold;

in vec4 v_color;

out vec4 PixelColor;

void main()
{
	if (v_color.r >= threshold)
		PixelColor = vec4(0.95, 0.1, 0.05, 1.0);
	else
		PixelColor = vec4(0.1, 0.1, 0.1, 1.0);
}
