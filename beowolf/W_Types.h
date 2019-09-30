//-----------------------------------------------------------------------------
// File:			W_Types.h
// Original Author:	Gordon Wood
//
// Common types throughout wolf (and some defines too)
//-----------------------------------------------------------------------------
#ifndef W_TYPES_H
#define W_TYPES_H

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GL/glfw.h>
#include <lib/glm/glm.hpp>
#include <lib/glm/gtc/matrix_transform.hpp>
#include <lib/glm/gtx/transform.hpp>
#include <lib/glm/gtc/type_ptr.hpp>

namespace wolf
{

// Our attribute indices 
enum Attribute
{
	AT_Position = 0,
	AT_Color,
	AT_TexCoord1,
	AT_TexCoord2,
	AT_TexCoord3,
	AT_TexCoord4,
	AT_TexCoord5,
	AT_TexCoord6,
	AT_TexCoord7,
	AT_TexCoord8,
	AT_Normal,
	AT_Tangent,
	AT_BoneIndices,
	AT_BoneWeight,
	AT_NUM_ATTRIBS
};

enum ComponentType
{
	CT_Float = 0,
	CT_Int,	
	CT_UInt,	
	CT_Byte,	
	CT_UByte,
	CT_UByte4,
	CT_ByteNorm,
	CT_UByteNorm,
	CT_Short,	
	CT_UShort,
	CT_ShortNorm,
	CT_UShortNorm,
	AT_NUM_COMPONENT_TYPES,
	CT_Invalid
};

enum DepthFunc
{
	DF_Never,
	DF_Less,
	DF_LessEqual,
	DF_Equal,
	DF_Greater,
	DF_GreaterEqual,
	DF_NotEqual,
	DF_Always,
	DF_NUM_DEPTH_FUNCS
};

enum BlendMode
{
	BM_SrcAlpha,			
	BM_One,				
	BM_SrcColor,	
	BM_OneMinusSrcColor,
	BM_OneMinusSrcAlpha,
	BM_DstAlpha,	
	BM_OneMinusDstAlpha,
	BM_DstColor,	
	BM_OneMinusDstColor,
	BM_Zero,		
	BM_NUM_BLEND_MODES
};

enum BlendEquation
{
	BE_Add,
	BE_Subtract,
	BE_ReverseSubtract,
	BE_NUM_BLEND_EQUATIONS
};

struct Color4
{
	float r,g,b,a;

	Color4(float p_fR, float p_fG, float p_fB, float p_fA) : r(p_fR), g(p_fG), b(p_fB), a(p_fA) {}
};

struct Plane
{
	// point-normal form description of a plane
	float a, b, c, d;

	Plane() { a = 0; b = 1; c = 0; d = 0; }
	Plane(float pA, float pB, float pC, float pD) : a(pA), b(pB), c(pC), d(pD) {}

	// helpful for some operations, like generating frustums
	float &operator [] (int i)
	{
		switch (i)
		{
		case 0:
			return a;
		case 1:
			return b;
		case 2:
			return c;
		case 3:
			return d;
		}
	}
};

#ifndef PI
#define PI		3.141592653589793238f
#endif
    
#define DEG_TO_RAD(d)  (((d)/180.0f)*PI)
#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)

#define ull unsigned long long

}

#endif

