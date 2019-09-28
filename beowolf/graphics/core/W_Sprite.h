//-----------------------------------------------------------------------------
// File:			W_Sprite.h
// Original Author:	Carel Boers
//
// Class representing a 2D textured quad.
//
//-----------------------------------------------------------------------------
#ifndef W_SPRITE_H
#define W_SPRITE_H

#include "W_Types.h"
#include "W_VertexBuffer.h"
#include "W_IndexBuffer.h"
#include "W_BufferManager.h"
#include "W_Material.h"
#include "W_VertexDeclaration.h"
#include "W_TextureManager.h"
#include "W_ProgramManager.h"
#include <string>
#include <map>
#include <vector>

namespace wolf
{
class Sprite
{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		Sprite(const std::string& p_strTexture, const std::string& p_strVertexProgram, const std::string& p_strFragmentProgram, const glm::vec2& p_vDimensions);
		~Sprite();

		void Update(float p_fDelta);
		void Render(const glm::mat4& p_mProj);

		void SetTransform(const glm::mat4& p_mWorldTransform) { m_mWorldTransform = p_mWorldTransform; }
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------

		wolf::VertexBuffer*			m_pVB;
		wolf::VertexDeclaration*	m_pDecl;
		wolf::Program*				m_pProgram;
		GLuint						m_uiTex;

		glm::mat4					m_mWorldTransform;

		//-------------------------------------------------------------------------
};

}

#endif



