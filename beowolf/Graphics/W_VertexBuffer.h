//-----------------------------------------------------------------------------
// File:			W_VertexBuffer.h
// Original Author:	Gordon Wood
//
// Derived class from wolf::Buffer, this one describing specifically a vertex
// buffer
//-----------------------------------------------------------------------------
#ifndef W_VERTEXBUFFER_H
#define W_VERTEXBUFFER_H

#include "W_Types.h"
#include "W_Buffer.h"

namespace wolf
{
class VertexBuffer : public Buffer
{
	friend class BufferManager;
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		virtual void Bind();
		virtual void Write(const void* p_pData, int p_iLength = -1);
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// Made private to enforce creation and deletion via BufferManager
		VertexBuffer(unsigned int p_uiLength);
		VertexBuffer(const void* p_pData, unsigned int p_uiLength);
		virtual ~VertexBuffer();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		unsigned int		m_uiLength;	
		GLuint				m_uiBuffer;
		//-------------------------------------------------------------------------
};

}

#endif
