#version 150

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world[64];
uniform mat3 WorldIT;
uniform mat4 lightSpaceMatrix;

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
	vec3 FragPos = vec3(world[gl_InstanceID] * a_position);
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	v_uv1 = a_uv1;
	v_normal = WorldIT * a_normal;
	v_color = a_color;
	vec4 tempPos = world[gl_InstanceID] * a_position;
	v_height = tempPos.y;
	gl_Position = projection * view * world[gl_InstanceID] * a_position;
}

