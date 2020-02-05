#version 150

uniform float threshold;

in vec4 v_color;

out vec4 PixelColor;

vec4 GetColor(float threshold) {
	if (threshold > 0.5)
		return vec4(min(0.1 + 1.0 - (threshold - 0.5) * 2, 0.95), 0.8, 0.05, 1.0);
	else
		return vec4(0.95, threshold * 1.6, 0.05, 1.0);
}

void main()
{
	if (v_color.r >= threshold)
		PixelColor = GetColor(1 - threshold);
	else
		PixelColor = vec4(0.1, 0.1, 0.1, 0.75);
}
