#version 150

uniform sampler2D tex;
uniform float threshold;

in vec4 v_color;
in vec2 v_uv1;

out vec4 PixelColor;

void main()
{
	vec4 color = texture(tex, v_uv1);
	if (v_color.r >= threshold)
		PixelColor = vec4(0.9, 0.9, 0.1, 1.0);
	else
		PixelColor = vec4(0.1, 0.1, 0.1, 0.75);
}
