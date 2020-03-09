#version 150
uniform mat4 projection;

in vec3 a_position;
in vec2 a_uv1;

out vec2 TexCoords;

void main()
{
   TexCoords = a_uv1;
   gl_Position = vec4(a_position.x, a_position.z, 0.0, 1.0);
}