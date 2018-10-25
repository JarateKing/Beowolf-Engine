//-----------------------------------------------------------------------------
// File:			W_VertexDeclaration.h
// Original Author:	Gordon Wood
//
// Class representing a complete vertex declaration, including all the
// attributes that comprise the vertex, and the associated source buffers
//-----------------------------------------------------------------------------
#ifndef W_VERTEXDECLARATION_H
#define W_VERTEXDECLARATION_H

#include <vector>
#include "W_Types.h"

namespace wolf
{
class VertexBuffer;
class IndexBuffer;

class VertexDeclaration
{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		VertexDeclaration();
		~VertexDeclaration();
	
		void Begin();
		void SetVertexBuffer(wolf::VertexBuffer* p_pVB);
		void SetIndexBuffer(wolf::IndexBuffer* p_pIB);
		void AppendAttribute(wolf::Attribute p_attr, int p_iNumComponents, wolf::ComponentType p_type, int p_iOffset = -1);
		void End();

		void Bind();
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE TYPES
		//-------------------------------------------------------------------------
		struct AttributeInfo
		{
			wolf::Attribute		m_attr;
			wolf::ComponentType	m_type;
			int					m_iOffset;
			int					m_iNumComponents;
		};
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		std::vector<AttributeInfo>	m_attrs;
		wolf::VertexBuffer*			m_pVB;
		wolf::IndexBuffer*			m_pIB;
		GLuint						m_vao;
		//-------------------------------------------------------------------------
};

}

#endif

