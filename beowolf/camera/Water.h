#ifndef WATER_H
#define WATER_H

#include "W_Common.h"
#include "W_Texture.h"
#include "W_TextureManager.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_VertexDeclaration.h"

class Water
{
public:

	Water();
	~Water();
	void Render(glm::mat4 projView, wolf::RenderFilterType type);
	void SetPos(glm::vec3 pos);

private:

	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_dProgram;
	wolf::Texture* m_tex;
	glm::vec3 m_pos;

	wolf::Vertex planeVertices[6] = {
		{ 500.0f, 5.0f,  500.0f, 0, 1, 0, 1, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f},
		{-500.0f, 5.0f,  500.0f, 0, 1, 0, 1, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
		{-500.0f, 5.0f, -500.0f, 0, 1, 0, 1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
		{ 500.0f, 5.0f,  500.0f, 0, 1, 0, 1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
		{-500.0f, 5.0f, -500.0f, 0, 1, 0, 1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
		{ 500.0f, 5.0f, -500.0f, 0, 1, 0, 1, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f}
	};

};

#endif TESTQUAD_H