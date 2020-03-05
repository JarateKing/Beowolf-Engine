#version 150
uniform mat4 projection;
uniform float time;
uniform vec3 scrollAngle;

in vec4 a_position;
in vec2 a_uv1;

out vec2 v_uv1;

void main()
{
    gl_Position = projection * a_position;
	v_uv1 = vec2(a_uv1.x + scrollAngle.x * time, a_uv1.y + scrollAngle.y * time);
}
