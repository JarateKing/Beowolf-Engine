//------------------------------------------------------------------------
// LineDrawer
//
// Created:	2013/02/18
// Author:	Carel Boers
//	
// A simple line drawing utility. This isn't the most efficient class so 
// don't plan to use for anything other than debug drawing...
//------------------------------------------------------------------------

#include "W_BufferManager.h"
#include "W_Common.h"
#include "W_LineDrawer.h"
#include "W_ProgramManager.h"

using namespace wolf;

//------------------------------------------------------------------------------
// Method:    LineDrawer
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
LineDrawer::LineDrawer()
	:
	m_pProgram(NULL),
	m_pVB(NULL),
	m_pDecl(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~LineDrawer
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
LineDrawer::~LineDrawer()
{
	wolf::BufferManager::DestroyBuffer(m_pVB);
	m_pVB = NULL;

	delete m_pDecl;
	m_pDecl = NULL;	
	
	wolf::ProgramManager::DestroyProgram(m_pProgram);
	m_pProgram = NULL;
}


//------------------------------------------------------------------------------
// Method:    Init
// Parameter: const std::string & p_strVertexProgramPath
// Parameter: const std::string & p_strFragmentProgramPath
// Returns:   void
// 
// Initializes the line drawer.
//------------------------------------------------------------------------------
void LineDrawer::Init(const std::string &p_strVertexProgramPath, const std::string &p_strFragmentProgramPath)
{	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	m_pProgram = wolf::ProgramManager::CreateProgram(p_strVertexProgramPath, p_strFragmentProgramPath);
}

//------------------------------------------------------------------------------
// Method:    Render
// Parameter: const glm::mat4 & p_mProjectionMatrix
// Parameter: const glm::mat4 & p_mViewMatrix
// Returns:   void
// 
// Render the list of lines.
//------------------------------------------------------------------------------
void LineDrawer::Render(const glm::mat4& p_mProjectionMatrix, const glm::mat4& p_mViewMatrix)
{
	size_t len = m_lVertexVector.size();
	if (len > 0)
	{
		Vertex* lineVerts = new Vertex[m_lVertexVector.size()];
		for (int i = 0; i < len; ++i)
		{
			LineData* data = m_lVertexVector[i];
			Vertex* vert = &lineVerts[i];
			vert->x = data->x;
			vert->y = data->y;
			vert->z = data->z;
			vert->r = data->r;
			vert->g = data->g;
			vert->b = data->b;
			vert->a = data->a;
		}

		m_pVB = wolf::BufferManager::CreateVertexBuffer(lineVerts, sizeof(LineData) * len);

		m_pDecl = new wolf::VertexDeclaration();
		m_pDecl->Begin();
		m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
		m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_Float);
		m_pDecl->SetVertexBuffer(m_pVB);
		m_pDecl->End();

		// Use shader program.
		m_pProgram->Bind();

		// Bind Uniforms
		m_pProgram->SetUniform("projection", p_mProjectionMatrix);
		m_pProgram->SetUniform("view", p_mViewMatrix);

		// Set up source data
		m_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_LINES, 0, len);

		delete[] lineVerts;
	}

	// Clear lines
	LineVector::iterator it = m_lVertexVector.begin(), end = m_lVertexVector.end();
	for (; it != end; ++it)
	{
		LineData* pVert = static_cast<LineData*>(*it);
		delete pVert;
		pVert = NULL;
	}
	m_lVertexVector.clear();
}

//------------------------------------------------------------------------------
// Method:    AddLine
// Parameter: const glm::vec3 & p_vStart
// Parameter: const glm::vec3 & p_vTo
// Parameter: const wolf::Color4 & p_cColor
// Returns:   void
// 
// Adds a line to be drawn.
//------------------------------------------------------------------------------
void LineDrawer::AddLine(const glm::vec3& p_vFrom, const glm::vec3& p_vTo, const wolf::Color4& p_cColor)
{
	LineData* vFrom = new LineData(p_vFrom.x, p_vFrom.y, p_vFrom.z, p_cColor.r, p_cColor.g, p_cColor.b, p_cColor.a);
	LineData* vTo = new LineData(p_vTo.x, p_vTo.y, p_vTo.z, p_cColor.r, p_cColor.g, p_cColor.b, p_cColor.a);
	m_lVertexVector.push_back(vFrom);
	m_lVertexVector.push_back(vTo);
}