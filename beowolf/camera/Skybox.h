#ifndef SKYBOX_H
#define SKYBOX_H

#include "W_Common.h"
#include "W_Texture.h"
#include "W_TextureManager.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_VertexDeclaration.h"

class Skybox
{
public:

	Skybox();
	~Skybox();
	void Render(glm::mat4 projView, wolf::RenderFilterType type);
	void SetPos(glm::vec3 pos);

private:

	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_dProgram;
	wolf::Texture* m_tex;
	glm::vec3 m_pos;

	wolf::Vertex planeVertices[36] = {
		{ 500.0f, -500.0f,  500.0f, 0, 1, 0, 1, 0.25f, 0.3333333f, 1.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f,  500.0f, 0, 1, 0, 1, 0.50f, 0.3333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.50f, 0.0000000f, 0.0f, 1.0f, 0.0f},
		{ 500.0f, -500.0f,  500.0f, 0, 1, 0, 1, 0.25f, 0.3333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.50f, 0.0000000f, 0.0f, 1.0f, 0.0f},
		{ 500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.25f, 0.0000000f, 0.0f, 1.0f, 0.0f},

		{500.0f,  500.0f,  500.0f, 0, 1, 0, 1, 0.25f, 0.66666666f, 1.0f, 1.0f, 0.0f},
		{500.0f, -500.0f,  500.0f, 0, 1, 0, 1, 0.25f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.00f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{500.0f,  500.0f,  500.0f, 0, 1, 0, 1, 0.25f, 0.66666666f, 0.0f, 1.0f, 0.0f},
		{500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.00f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{500.0f,  500.0f, -500.0f, 0, 1, 0, 1, 0.00f, 0.66666666f, 0.0f, 1.0f, 0.0f},

		{-500.0f,  500.0f,  500.0f, 0, 1, 0, 1, 0.50f, 0.66666666f, 1.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f,  500.0f, 0, 1, 0, 1, 0.50f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.75f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f,  500.0f,  500.0f, 0, 1, 0, 1, 0.50f, 0.66666666f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.75f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f,  500.0f, -500.0f, 0, 1, 0, 1, 0.75f, 0.66666666f, 0.0f, 1.0f, 0.0f},

		{ 500.0f,  500.0f, 500.0f, 0, 1, 0, 1, 0.25f, 0.66666666f, 1.0f, 1.0f, 0.0f},
		{ 500.0f, -500.0f, 500.0f, 0, 1, 0, 1, 0.25f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, 500.0f, 0, 1, 0, 1, 0.50f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{ 500.0f,  500.0f, 500.0f, 0, 1, 0, 1, 0.25f, 0.66666666f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, 500.0f, 0, 1, 0, 1, 0.50f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f,  500.0f, 500.0f, 0, 1, 0, 1, 0.50f, 0.66666666f, 0.0f, 1.0f, 0.0f},

		{ 500.0f,  500.0f, -500.0f, 0, 1, 0, 1, 1.00f, 0.66666666f, 1.0f, 1.0f, 0.0f},
		{ 500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 1.00f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.75f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{ 500.0f,  500.0f, -500.0f, 0, 1, 0, 1, 1.00f, 0.66666666f, 0.0f, 1.0f, 0.0f},
		{-500.0f, -500.0f, -500.0f, 0, 1, 0, 1, 0.75f, 0.33333333f, 0.0f, 1.0f, 0.0f},
		{-500.0f,  500.0f, -500.0f, 0, 1, 0, 1, 0.75f, 0.66666666f, 0.0f, 1.0f, 0.0f},

		{ 500.0f, 500.0f,  500.0f, 0, 1, 0, 1, 0.25f, 0.66666666f, 1.0f, 1.0f, 0.0f},
		{-500.0f, 500.0f,  500.0f, 0, 1, 0, 1, 0.50f, 0.66666666f, 0.0f, 1.0f, 0.0f},
		{-500.0f, 500.0f, -500.0f, 0, 1, 0, 1, 0.50f, 1.00000000f, 0.0f, 1.0f, 0.0f},
		{ 500.0f, 500.0f,  500.0f, 0, 1, 0, 1, 0.25f, 0.66666666f, 0.0f, 1.0f, 0.0f},
		{-500.0f, 500.0f, -500.0f, 0, 1, 0, 1, 0.50f, 1.00000000f, 0.0f, 1.0f, 0.0f},
		{ 500.0f, 500.0f, -500.0f, 0, 1, 0, 1, 0.25f, 1.00000000f, 0.0f, 1.0f, 0.0f},
	};

};

#endif SKYBOX_H