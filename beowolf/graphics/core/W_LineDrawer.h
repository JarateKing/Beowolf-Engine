//------------------------------------------------------------------------
// LineDrawer
//
// Created:	2013/02/18
// Author:	Carel Boers
//	
// A simple line drawing utility. This isn't the most efficient class so 
// don't plan to use for anything other than debug drawing...
//------------------------------------------------------------------------

#ifndef W_LINEDRAWER_H
#define W_LINEDRAWER_H

#include "W_Program.h"
#include "W_VertexBuffer.h"
#include "W_VertexDeclaration.h"
#include <vector>

namespace wolf
{
class LineDrawer
{
private:
	//-------------------------------------------------------------------------
	// Private types
	//-------------------------------------------------------------------------
	struct LineData
	{
		GLfloat x,y,z;
		GLfloat r, g, b, a;
		LineData(float _x, float _y, float _z, float _r, float _g, float _b, float _a)
			: x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), a(_a) { }
	};
	typedef std::vector<LineData*> LineVector;

	struct Vertex
	{
		GLfloat x,y,z;
		GLfloat r, g, b, a;
	};

public:
	//-------------------------------------------------------------------------
	// Public methods.
	//-------------------------------------------------------------------------
	LineDrawer();
	~LineDrawer();

	void Init(const std::string &p_strVertexProgramPath, const std::string &p_strFragmentProgramPath);
	void Render(const glm::mat4& p_mProjectionMatrix, const glm::mat4& p_mViewMatrix);
	void AddLine(const glm::vec3& p_vStart, const glm::vec3& p_vTo, const wolf::Color4& p_cColor = wolf::Color4(1.0f, 1.0f, 1.0f, 1.0f));

private:
	//-------------------------------------------------------------------------
	// Private members.
	//-------------------------------------------------------------------------

	// Shader
	wolf::Program* m_pProgram;

	// Vertex buffer and declaration
	wolf::VertexBuffer* m_pVB;
	wolf::VertexDeclaration* m_pDecl;

	// List of lines to draw; cleared each frame after being rendered
	LineVector m_lVertexVector;
};
}

#endif
