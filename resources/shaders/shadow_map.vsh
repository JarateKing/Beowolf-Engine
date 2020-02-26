#version 150

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

in vec3 a_position;
in vec4 a_color;
in vec2 a_uv1;
in vec3 a_normal;

void main()
{
	gl_Position = lightSpaceMatrix * model * vec4(a_position, 1.0);
}