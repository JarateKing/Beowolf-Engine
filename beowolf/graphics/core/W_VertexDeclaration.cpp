//-----------------------------------------------------------------------------
// File:			W_VertexDeclaration.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_VertexDeclaration.h"
#include "W_VertexBuffer.h"
#include "W_IndexBuffer.h"
#include "W_Types.h"

namespace wolf
{
GLenum g_aTypeMap[] =
{
	GL_FLOAT,				// CT_Float
	GL_INT,					// CT_Int,
	GL_UNSIGNED_INT,		// CT_UInt,	
	GL_BYTE,				// CT_Byte,	
	GL_UNSIGNED_BYTE,		// CT_UByte
	GL_UNSIGNED_BYTE,		// CT_UByte4
	GL_BYTE,				// CT_ByteNorm,
	GL_UNSIGNED_BYTE,		// CT_UByteNorm,
	GL_SHORT,				// CT_Short,	
	GL_UNSIGNED_SHORT,		// CT_UShort,
	GL_SHORT,				// CT_ShortNorm,
	GL_UNSIGNED_SHORT,		// CT_UShortNorm,
};

int g_aSizeMap[] =
{
	sizeof(float),			// CT_Float
	sizeof(int),			// CT_Int,
	sizeof(int),			// CT_UInt,	
	sizeof(char),			// CT_Byte
	sizeof(char),			// CT_UByte
	sizeof(char)*4,			// CT_UByte4
	sizeof(char),			// CT_ByteNorm,
	sizeof(char),			// CT_UByteNorm,
	sizeof(short),			// CT_Short,	
	sizeof(short),			// CT_UShort,
	sizeof(short),			// CT_ShortNorm,
	sizeof(short)			// CT_UShortNorm,
};

GLboolean g_aNormalizeMap[] =
{
	GL_FALSE,			// CT_Float
	GL_FALSE,			// CT_Int,
	GL_FALSE,			// CT_UInt,	
	GL_FALSE,			// CT_Byte
	GL_FALSE,			// CT_UByte
	GL_FALSE,			// CT_UByte4
	GL_TRUE,			// CT_ByteNorm,
	GL_TRUE,			// CT_UByteNorm,
	GL_FALSE,			// CT_Short,	
	GL_FALSE,			// CT_UShort,
	GL_TRUE,			// CT_ShortNorm,
	GL_TRUE				// CT_UShortNorm,
};

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
VertexDeclaration::VertexDeclaration() : m_pVB(0), m_pIB(0), m_vao(0)
{
	glGenVertexArrays(1, &m_vao);
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
VertexDeclaration::~VertexDeclaration()
{
	glDeleteVertexArrays(1, &m_vao);
}

//----------------------------------------------------------
// Method that must be called before any calls to AppendAttribute
// or SetVertexBuffer or SetIndexBuffer. Once finished changing
// the declaration, call End()
//----------------------------------------------------------
void VertexDeclaration::Begin()
{
	glBindVertexArray(m_vao);
}

//----------------------------------------------------------
// Method that must be called after all calls to AppendAttribute
// or SetVertexBuffer or SetIndexBuffer. 
//----------------------------------------------------------
void VertexDeclaration::End()
{
	if( m_pVB )
		m_pVB->Bind();
	if( m_pIB )
		m_pIB->Bind();

	// Work out total size of vertex
	int iVertSize = 0;
	for(unsigned int i = 0; i < m_attrs.size(); i++)
	{
		iVertSize += m_attrs[i].m_iNumComponents * g_aSizeMap[ m_attrs[i].m_type ];
	}

	int iOfs = 0;
	for(unsigned int i = 0; i < m_attrs.size(); i++)
	{
		const AttributeInfo& info = m_attrs[i];

		if( info.m_iOffset != -1 )
			iOfs = info.m_iOffset;

		glVertexAttribPointer(info.m_attr,
							  info.m_iNumComponents,
							  g_aTypeMap[info.m_type],
							  info.m_attr == AT_Color ? true : g_aNormalizeMap[info.m_type],
							  iVertSize,
							  (void*)iOfs);
		glEnableVertexAttribArray(info.m_attr);

		if( info.m_iOffset == -1 )
			iOfs += g_aSizeMap[ info.m_type ] * info.m_iNumComponents;
	}
}

//----------------------------------------------------------
// Adds and describes a vertex attribute to this vertex declaration.
//----------------------------------------------------------
void VertexDeclaration::AppendAttribute(wolf::Attribute p_attr, int p_iNumComponents, wolf::ComponentType p_type, int p_iOffset)
{
	AttributeInfo info;
	info.m_attr				= p_attr;
	info.m_type				= p_type;
	info.m_iNumComponents	= p_iNumComponents;
	info.m_iOffset			= p_iOffset;
	m_attrs.push_back(info);
}

//----------------------------------------------------------
// Applies this vertex declaration, meaning it sets up the
// driver's vertex attributes to be what we need them to be for this
// description of a vertex
//----------------------------------------------------------
void VertexDeclaration::Bind()
{
    glBindVertexArray(m_vao);
    m_pVB->Bind();
    if( m_pIB )
        m_pIB->Bind();
}

//----------------------------------------------------------
// Sets the Vertex Buffer that all the attributes are stored
// in. Note that this class requires that all attributes be
// stored in the same vertex buffer, even though OpenGL doesn't
// require this. This is to simplify API design by removing a
// seldom-used (for us) feature.
//----------------------------------------------------------
void VertexDeclaration::SetVertexBuffer(wolf::VertexBuffer* p_pVB)
{
	m_pVB = p_pVB;
}

//----------------------------------------------------------
// Sets the Index Buffer to be used whenever we bind this 
// vertex declaration. If this isn't called, it is assumed
// that no Index Buffer is required.
//----------------------------------------------------------
void VertexDeclaration::SetIndexBuffer(wolf::IndexBuffer* p_pIB)
{
	m_pIB = p_pIB;
}

}
