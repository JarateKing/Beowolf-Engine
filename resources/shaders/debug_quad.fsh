#version 150
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

void main()
{             
    float depthValue = texture(depthMap, TexCoords).x;
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}