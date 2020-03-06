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
	void Render(glm::mat4 projView, wolf::RenderFilterType type, unsigned int reflectionTex);
	void Update(float delta);
	void SetPos(glm::vec3 pos);

private:

	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_dProgram;
	wolf::Texture* m_tex;
	wolf::Texture* m_normals;
	glm::vec3 m_pos;
	float m_time = 0;
	glm::vec3 m_scrollAngle1 = glm::vec3(0.01f, 0.05f, 1.0f);
	glm::vec3 m_scrollAngle2 = glm::vec3(0.09f, 0.04f, 1.0f);
	float m_copyScale = 0.8f;

	wolf::Vertex planeVertices[6] = {
		{ 700.0f, 5.0f,  700.0f, 0, 1, 0, 1, 0.0f, 70.0f,	0.0f, 1.0f, 0.0f},
		{-700.0f, 5.0f,  700.0f, 0, 1, 0, 1, 70.0f, 70.0f,	0.0f, 1.0f, 0.0f},
		{-700.0f, 5.0f, -700.0f, 0, 1, 0, 1, 70.0f, 0.0f,	0.0f, 1.0f, 0.0f},
		{ 700.0f, 5.0f,  700.0f, 0, 1, 0, 1, 0.0f, 70.0f,	0.0f, 1.0f, 0.0f},
		{-700.0f, 5.0f, -700.0f, 0, 1, 0, 1, 70.0f, 0.0f,	0.0f, 1.0f, 0.0f},
		{ 700.0f, 5.0f, -700.0f, 0, 1, 0, 1, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f}
	};

};

#endif TESTQUAD_H