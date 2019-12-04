#version 150

uniform sampler2D tex;

in vec2 v_uv1;
out vec4 PixelColor;

void main()
{
	PixelColor = vec4(v_uv1[0], v_uv1[1], 1.0, 1.0);
}
