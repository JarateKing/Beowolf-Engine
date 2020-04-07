#ifndef TESTQUAD_H
#define TESTQUAD_H

#include "W_Common.h"
#include "W_Texture.h"
#include "W_TextureManager.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_VertexDeclaration.h"

class TestQuad
{
public:

	TestQuad();
	~TestQuad();
	void Render(glm::mat4 projView, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTex);

private:

	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_dProgram;

	wolf::Vertex planeVertices[6] = {
		{25.0f, -0.5f, 75.0f, 0, 1, 0, 1, 0.0f,  1.0f, 1.0f, 1.0f, 0.0f},
		{-25.0f, -0.5f,  75.0f, 0, 1, 0, 1, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
		{-25.0f, -0.5f, 25.0f, 0, 1, 0, 1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},

		{ 25.0f, -0.5f,  75.0f, 0, 1, 0, 1, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f},
		{-25.0f, -0.5f, 25.0f, 0, 1, 0, 1,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
		{ 25.0f, -0.5f, 25.0f, 0, 1, 0, 1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}
	};

};

#endif