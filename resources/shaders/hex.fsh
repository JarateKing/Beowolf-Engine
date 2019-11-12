#version 150
in vec4 v_color;
in vec2 v_uv1;

uniform sampler2D tex;

out vec4 PixelColor;

void main()
{
    PixelColor = texture(tex, v_uv1) * v_color;
}


