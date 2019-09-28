#version 150
uniform mat4 projection;
in vec4 a_position;

void main()
{
    gl_Position = projection * a_position;
}
