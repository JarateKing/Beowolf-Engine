#version 150

uniform mat4 BoneMatrixArray[128];
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

in vec4 a_position;
in vec4 a_color;
in vec2 a_uv1;
in vec3 a_normal;
in vec4 a_boneIndices;
in vec4 a_boneWeights;

void main()
{
	ivec4 boneIndices = ivec4(a_boneIndices);
	vec4 blendedPos = vec4(0,0,0,0);
	blendedPos += BoneMatrixArray[boneIndices.x] * a_position * a_boneWeights.x;
	blendedPos += BoneMatrixArray[boneIndices.y] * a_position * a_boneWeights.y;
	blendedPos += BoneMatrixArray[boneIndices.z] * a_position * a_boneWeights.z;
	blendedPos += BoneMatrixArray[boneIndices.w] * a_position * a_boneWeights.w;
	
	gl_Position = lightSpaceMatrix * model * blendedPos;
}