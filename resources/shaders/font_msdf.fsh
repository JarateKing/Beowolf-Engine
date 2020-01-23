#version 150
in vec2 v_uv1;

uniform sampler2D tex;
uniform vec4 color;
uniform float fontsize;

out vec4 PixelColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

vec3 subpixel(float v, float a) {
	float vt = 0.6 * v;
	vec3 rgb_max = vec3(-vt, 0, vt);
	float top = abs(vt);
	float bottom = -top - 1.0;
	float cfloor = mix(top, bottom, a);
	vec3 res = clamp(rgb_max - vec3(cfloor), 0.0, 1.0);
	return res;
}

void main()
{	
	float hint_amount = 1.0;
	float subpixel_amount = 1.0;
	float doffset = 1.0 / (2.0 * fontsize / 56.0 * 1080.0 / 512.0 * 16.0 / 9.0);
	vec3 bg_color = vec3(0.352, 0.352, 0.352);

	// get samples
	vec2 v_uv1_north = vec2(v_uv1.x, v_uv1.y + 1.0 / 512.0);
	vec2 v_uv1_east = vec2(v_uv1.x + 1.0 / 1024.0, v_uv1.y);
	
	vec4 ssdf = texture(tex, v_uv1);
	vec4 ssdf_north = texture(tex, v_uv1_north);
	vec4 ssdf_east = texture(tex, v_uv1_east);
	
	float sdf = median(ssdf.r, ssdf.g, ssdf.b);
	float sdf_north = median(ssdf_north.r, ssdf_north.g, ssdf_north.b);
	float sdf_east = median(ssdf_east.r, ssdf_east.g, ssdf_east.b);
	
	// get gradient direction
	vec2 sgrad = vec2(sdf_east - sdf, sdf_north - sdf);
	float sgrad_len = max(length(sgrad), 1.0 / 128.0);
	vec2 grad = sgrad / vec2(sgrad_len);
	float vgrad = abs(grad.y); // 0 = vertical, 1 = horizontal
	
	float horz_scale = 1.0; //1.1
	float vert_scale = 1.0; //0.6
	float hdoffset = mix(doffset * horz_scale, doffset * vert_scale, vgrad);
	float res_doffset = mix(doffset, hdoffset, hint_amount);
	
	float alpha = smoothstep(0.5 - res_doffset, 0.5 + res_doffset, sdf);
	float other_alpha = 1.0;
	
	// remove some artifacting
	if (alpha < 5.0 / 256.0) {
		other_alpha = 0.0;
	}
	
	vec3 channels = subpixel(grad.x * 0.5 * subpixel_amount, alpha);
	vec3 res = mix(bg_color, color.rgb, channels);
	
    PixelColor = vec4(res.r, res.g, res.b, other_alpha * color.a);
}
