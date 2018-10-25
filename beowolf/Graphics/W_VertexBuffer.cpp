//-----------------------------------------------------------------------------
// File:			W_VertexBuffer.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_VertexBuffer.h"

namespace wolf
{
//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
VertexBuffer::VertexBuffer(unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
	glGenBuffers(1, &m_uiBuffer);
}

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
VertexBuffer::VertexBuffer(const void* p_pData, unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
	glGenBuffers(1, &m_uiBuffer);
	Write(p_pData);
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1,&m_uiBuffer);
}

//----------------------------------------------------------
// Fills this vertex buffer with the given data
//----------------------------------------------------------
void VertexBuffer::Write(const void* p_pData, int p_iLength)
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER, p_iLength == -1 ? m_uiLength : p_iLength, p_pData, GL_STATIC_DRAW);
}

//----------------------------------------------------------
// Binds this buffer
//----------------------------------------------------------
void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
}

}
