#version 150
uniform mat4 projection;
uniform float time;
uniform vec3 scrollAngle1;
uniform vec3 scrollAngle2;
uniform float copyScale;

in vec4 a_position;
in vec2 a_uv1;

out vec2 v_uv1;
out vec2 v_uv2;

void main()
{
    gl_Position = projection * a_position;
	v_uv1 = vec2(a_uv1.x + scrollAngle1.x * time, a_uv1.y + scrollAngle1.y * time);
	v_uv2 = vec2(a_uv1.x * copyScale + scrollAngle2.x * time, a_uv1.y * copyScale + scrollAngle2.y * time);
}
