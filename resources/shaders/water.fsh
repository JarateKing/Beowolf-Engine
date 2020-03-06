#version 150

uniform sampler2D tex;
uniform sampler2D reflection;
uniform float screenX;
uniform float screenY;

in vec2 v_uv1;
in vec2 v_uv2;

out vec4 PixelColor;

void main()
{
	vec4 texVal = texture(tex, v_uv1) * texture(tex, v_uv2);
	float magnitude = texVal.r * texVal.r * texVal.r * 3.0;
	
	vec2 reflectCoord = vec2(gl_FragCoord.x / screenX, -gl_FragCoord.y / screenY);
	vec4 reflect = texture(reflection, reflectCoord);
	
	PixelColor = vec4(-0.1, 0.1, 0.15, 0) + 
				 vec4(reflect.r, reflect.g, reflect.b, 0.7) +
				 vec4(magnitude, magnitude, magnitude, magnitude);
}
