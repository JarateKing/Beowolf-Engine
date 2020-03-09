#version 150

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform mat4 lightSpaceMatrix;
uniform mat3 WorldIT;

in vec4 a_position;
in vec4 a_color;
in vec2 a_uv1;
in vec3 a_normal;

out vec4 FragPosLightSpace;
out vec2 v_uv1;
out vec3 v_normal;
out vec4 v_color;
out float v_height;

void main()
{
	vec3 FragPos = vec3(world * a_position);
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	v_normal = WorldIT * a_normal;
	v_color = a_color;
	v_uv1 = a_uv1;
	v_height = a_position.y;
	gl_Position = projection * world * a_position;
}
