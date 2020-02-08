#version 150

uniform vec4 color;
uniform float alpha;

in vec2 v_uv1;

out vec4 PixelColor;

void main()
{
	PixelColor = vec4(color.rgb, color.a * alpha);
}
