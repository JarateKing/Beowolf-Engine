#version 150
in vec2 v_uv1;

uniform sampler2D tex;
uniform vec4 color;

out vec4 PixelColor;

void main()
{
	vec4 d = texture(tex, v_uv1);
	float alpha = sqrt((d.x * d.x) + (d.y * d.y) + (d.z * d.z)) * (1/sqrt(3));
    PixelColor = color * vec4(1,1,1,alpha);
}
