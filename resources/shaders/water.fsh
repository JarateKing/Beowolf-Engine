#version 150

uniform sampler2D tex;
uniform sampler2D reflection;

in vec2 v_uv1;
in vec2 v_uv2;
in vec2 v_uv3;

out vec4 PixelColor;

void main()
{
	//vec4 texVal = texture(tex, v_uv1) * texture(tex, v_uv2);
	//float magnitude = texVal.r * texVal.r * texVal.r * 3.0;
	// PixelColor = vec4(0.3, 0.45, 0.7, 0.4) + vec4(magnitude, magnitude, magnitude, magnitude);
	PixelColor = texture(reflection, v_uv3);
}
