//-----------------------------------------------------------------------------
// File:			W_Sprite.h
// Original Author:	Carel Boers
//
// Class representing a 2D textured quad.
//
//-----------------------------------------------------------------------------
#include "W_Sprite.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_MaterialManager.h"

using namespace wolf;

struct Vertex
{
	GLfloat x,y;
	GLfloat u,v;
};

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
Sprite::Sprite(const std::string& p_strTexture, const std::string& p_strVertexProgram, const std::string& p_strFragmentProgram, const glm::vec2& p_vDimensions) 
	:
	m_pVB(NULL),
	m_pDecl(NULL),
	m_pProgram(NULL)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	const Vertex squareVertices[] = {
		{ 0.0f, 0.0f,	0, 1 }, 
		{ 0.0f, p_vDimensions.y,	0, 0 }, 
		{ p_vDimensions.x, p_vDimensions.y,	1, 0 }, 

		{ p_vDimensions.x, p_vDimensions.y,	1, 0 }, 
		{ p_vDimensions.x, 0.0f,	1, 1 }, 
		{ 0.0f, 0.0,	0, 1 }, 
	};

	m_pProgram = wolf::ProgramManager::CreateProgram(p_strVertexProgram, p_strFragmentProgram);
	m_pVB = wolf::BufferManager::CreateVertexBuffer(squareVertices, sizeof(Vertex) * 6);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();

	glGenTextures(1,&m_uiTex);
	glBindTexture(GL_TEXTURE_2D, m_uiTex);
	GLFWimage img;
	glfwReadImage(p_strTexture.c_str(), &img, 0 );
	glTexImage2D(GL_TEXTURE_2D, 0, img.Format, img.Width, img.Height, 0, img.Format, GL_UNSIGNED_BYTE, img.Data);
	glfwFreeImage(&img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
Sprite::~Sprite()
{
	wolf::BufferManager::DestroyBuffer(m_pVB);
	delete m_pDecl;
	wolf::ProgramManager::DestroyProgram(m_pProgram);
}

//----------------------------------------------------------
// Updates this sprite
//----------------------------------------------------------
void Sprite::Update(float p_fDelta)
{

}

//----------------------------------------------------------
// Renders this sprite
//----------------------------------------------------------
void Sprite::Render(const glm::mat4& p_mProj)
{
	// Use shader program.
	m_pProgram->Bind();

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, (int)m_uiTex);

	// Bind Uniforms
	m_pProgram->SetUniform("projection", p_mProj);
	m_pProgram->SetUniform("world", m_mWorldTransform);
	m_pProgram->SetUniform("texture", 0);

	// Set up source data
	m_pDecl->Bind();

	// Draw!
	glDrawArrays(GL_TRIANGLES, 0, 6);
}