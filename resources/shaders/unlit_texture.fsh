#version 150

uniform sampler2D tex;

in vec2 v_uv1;
out vec4 PixelColor;

void main()
{	
    PixelColor = texture(tex, v_uv1);
}
