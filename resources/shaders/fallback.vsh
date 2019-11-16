#version 150
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
in vec4 a_position;

void main()
{
    gl_Position = projection * view * world * a_position;
}
