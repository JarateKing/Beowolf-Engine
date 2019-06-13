//-----------------------------------------------------------------------------
// File:			W_Buffer.h
// Original Author:	Gordon Wood
//
// Base class for a buffer that is used by the rendering API. Concrete
// subclasses will be for an IndexBuffer and a VertexBuffer
//-----------------------------------------------------------------------------
#ifndef W_BUFFER_H
#define W_BUFFER_H

namespace wolf
{
class Buffer
{
	friend class BufferManager;
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		virtual void Bind() = 0;
		virtual void Write(const void* p_pData, int p_iLength = -1 ) = 0;
		//-------------------------------------------------------------------------

	protected:
		//-------------------------------------------------------------------------
		// PROTECTED MEMBERS
		//-------------------------------------------------------------------------
		// Made protected to enforce creation and deletion via BufferManager
		Buffer() {}
		virtual ~Buffer() {}
		//-------------------------------------------------------------------------
};

}

#endif
