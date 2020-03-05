#version 150
uniform mat4 projection;
uniform float time;
in vec4 a_position;
in vec2 a_uv1;
out vec2 v_uv1;

void main()
{
    gl_Position = projection * a_position;
	v_uv1 = vec2(a_uv1.x + 0.05 * time, a_uv1.y + 0.1 * time);
}
