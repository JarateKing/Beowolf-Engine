#version 150

uniform sampler2D tex;
uniform float alpha;

in vec2 v_uv1;

out vec4 PixelColor;

void main()
{
	vec4 clr = texture(tex, v_uv1);
	PixelColor = vec4(clr.rgb, clr.a * alpha);
}
