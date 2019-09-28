#version 150
uniform mat4 projection;
in vec4 a_position;
in vec4 a_color;
in vec2 a_uv1;
out vec4 v_color;
out vec2 v_uv1;

void main()
{
    gl_Position = projection * a_position;
	v_color = a_color;
	v_uv1 = a_uv1;
}