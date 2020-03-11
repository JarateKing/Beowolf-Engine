#version 150

in vec2 TexCoords;
out vec4 PixelColor;

uniform sampler2D postProcessingTex;
uniform float percent;

void main()
{
	vec4 c = texture(postProcessingTex, TexCoords);
	float L = dot(c.rgb, vec3(0.22, 0.707, 0.071));
	PixelColor.rgb = mix(c.rgb,vec3(L,L,L),percent);
	PixelColor.a = c.a;
}