#ifndef VERTEX_TYPE_H
#define VERTEX_TYPE_H

#include "W_Types.h"
#include "W_VertexDeclaration.h"

namespace wolf
{
	struct Vertex
	{
		GLfloat x, y, z;
		GLubyte r, g, b, a;
		GLfloat u, v;
		GLfloat normalDirX = 0.0f;
		GLfloat normalDirY = 1.0f;
		GLfloat normalDirZ = 0.0f;
		glm::vec4 boneIndices;
		glm::vec4 boneWeights;

		void setNormalDir(glm::vec3 input)
		{
			normalDirX = input.x;
			normalDirY = input.y;
			normalDirZ = input.z;
		}

		void setColor(glm::vec3 color)
		{
			r = color.r * 255;
			g = color.g * 255;
			b = color.b * 255;
		}

		void setColor(glm::vec4 color)
		{
			r = color.r * 255;
			g = color.g * 255;
			b = color.b * 255;
			a = color.a * 255;
		}

		glm::vec3 xyz()
		{
			return glm::vec3(x, y, z);
		}

		glm::vec2 xy()
		{
			return glm::vec2(x, y);
		}

		glm::vec2 xz()
		{
			return glm::vec2(x, z);
		}

		glm::vec2 yz()
		{
			return glm::vec2(y, z);
		}

		Vertex operator +(const Vertex other)
		{
			return {
				x + other.x,
				y + other.y,
				z + other.z,
				(GLubyte)(r + other.r),
				(GLubyte)(g + other.g),
				(GLubyte)(b + other.b),
				(GLubyte)(a + other.a),
				u + other.u,
				v + other.v
			};
		}

		Vertex operator -(const Vertex other)
		{
			return {
				x - other.x,
				y - other.y,
				z - other.z,
				(GLubyte)(r - other.r),
				(GLubyte)(g - other.g),
				(GLubyte)(b - other.b),
				(GLubyte)(a - other.a),
				u - other.u,
				v - other.v
			};
		}

		Vertex operator *(const Vertex other)
		{
			return {
				x * other.x,
				y * other.y,
				z * other.z,
				(GLubyte)(r * other.r),
				(GLubyte)(g * other.g),
				(GLubyte)(b * other.b),
				(GLubyte)(a * other.a),
				u * other.u,
				v * other.v
			};
		}

		static void applyAttributes(VertexDeclaration* decl)
		{
			decl->AppendAttribute(AT_Position, 3, CT_Float);
			decl->AppendAttribute(AT_Color, 4, CT_UByte);
			decl->AppendAttribute(AT_TexCoord1, 2, CT_Float);
			decl->AppendAttribute(AT_Normal, 3, CT_Float);
			decl->AppendAttribute(AT_BoneIndices, 4, CT_Float);
			decl->AppendAttribute(AT_BoneWeight, 4, CT_Float);
		}
	};
}

#endif
