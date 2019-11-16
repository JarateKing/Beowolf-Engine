#version 150
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
in vec4 a_position;
in vec4 a_color;
in vec2 a_uv1;
in vec3 a_normal;
out vec2 v_uv1;

void main()
{
    gl_Position = projection * view * world * a_position;
	v_uv1 = a_uv1;
}