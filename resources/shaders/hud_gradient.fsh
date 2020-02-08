#version 150

uniform float alpha;

in vec4 v_color;
in vec2 v_uv1;

out vec4 PixelColor;

void main()
{
	PixelColor = vec4(v_color.rgb, v_color.a * alpha);
}
