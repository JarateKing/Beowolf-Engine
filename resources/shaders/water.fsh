#version 150

uniform sampler2D tex;
uniform sampler2D reflection;
uniform sampler2D normaltex;
uniform float screenX;
uniform float screenY;

in vec2 v_uv1;
in vec2 v_uv2;
in vec2 v_uv3;

out vec4 PixelColor;

void main()
{
	vec4 texVal = texture(tex, v_uv1) * texture(tex, v_uv2);
	float magnitude = texVal.r * texVal.r * texVal.r * 3.0;
	
	vec4 offset = texture(normaltex, v_uv3);
	
	vec2 reflectCoord = vec2(gl_FragCoord.x / screenX + (offset.r - 0.5) * 0.015, 1 - gl_FragCoord.y / screenY + (offset.g - 0.5) * 0.015);
	vec4 reflect = texture(reflection, reflectCoord);
	
	PixelColor = vec4(-0.1, 0.1, 0.15, 0) + 
				 vec4(reflect.r, reflect.g, reflect.b, 0.7) +
				 vec4(magnitude, magnitude, magnitude, magnitude);
}
