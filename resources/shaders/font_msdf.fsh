#version 150
in vec2 v_uv1;

uniform sampler2D tex;
uniform vec4 color;

out vec4 PixelColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
	//vec4 d = texture(tex, v_uv1);
	//float alpha = sqrt((d.x * d.x) + (d.y * d.y) + (d.z * d.z)) * (1/sqrt(3));
	
	vec4 sample = texture(tex, v_uv1);
	float signDist = median(sample.r, sample.g, sample.b) - 0.5;
	float alpha = clamp(signDist / fwidth(signDist) + 0.5, 0.0, 1.0);
	
    PixelColor = color * vec4(1,1,1,alpha);
}
