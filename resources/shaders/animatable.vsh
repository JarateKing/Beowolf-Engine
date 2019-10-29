#version 150

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
//uniform mat3 WorldIT;
//uniform mat4 BoneMatrixArray[64];
//uniform mat3 BoneMatrixArrayIT[64];

in vec4 a_position;
in vec2 a_uv1;
in vec3 a_normal;
//in vec4 a_boneIndices;
//in vec4 a_boneWeights;

out vec2 v_uv1;
//out vec3 v_normal;

void main()
{
	//ivec4 boneIndices = ivec4(a_boneIndices);
	//vec4 blendedPos = vec4(0,0,0,0);
	//blendedPos += BoneMatrixArray[boneIndices.x] * a_position * a_boneWeights.x;
	//blendedPos += BoneMatrixArray[boneIndices.y] * a_position * a_boneWeights.y;
	//blendedPos += BoneMatrixArray[boneIndices.z] * a_position * a_boneWeights.z;
	//blendedPos += BoneMatrixArray[boneIndices.w] * a_position * a_boneWeights.w;
	
	//vec3 blendedNormal = vec3(0,0,0);
	//blendedNormal += BoneMatrixArrayIT[boneIndices.x] * a_normal * a_boneWeights.x;
	//blendedNormal += BoneMatrixArrayIT[boneIndices.y] * a_normal * a_boneWeights.y;
	//blendedNormal += BoneMatrixArrayIT[boneIndices.z] * a_normal * a_boneWeights.z;
	//blendedNormal += BoneMatrixArrayIT[boneIndices.w] * a_normal * a_boneWeights.w;


    //gl_Position = WorldViewProj * blendedPos;
	gl_Position = projection * view * world * a_position;
	v_uv1 = a_uv1;
    //v_normal = WorldIT * blendedNormal;
}
