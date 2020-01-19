#version 150

uniform vec3 modelColor;
in vec4 v_color;
out vec4 PixelColor;

void main()
{
	PixelColor = vec4((v_color.rgb * v_color.a) + (modelColor * (1 - v_color.a)), 1);
}
