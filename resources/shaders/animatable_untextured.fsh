#version 150

uniform vec3 modelColor;
uniform vec3 modelAdditive;
uniform vec3 modelFilter;
in vec4 v_color;
out vec4 PixelColor;

void main()
{
	PixelColor = vec4(((v_color.rgb * v_color.a) + (modelColor * (1 - v_color.a))) * modelFilter + modelAdditive, 1);
}
