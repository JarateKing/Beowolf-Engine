#ifndef POSTPROCESSINGQUAD_H
#define POSTPROCESSINGQUAD_H

#include "W_Common.h"
#include "W_Texture.h"
#include "W_TextureManager.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_VertexDeclaration.h"

class PostProcessingQuad
{
public:
	//Public Methods
	PostProcessingQuad();
	~PostProcessingQuad();
	void Render(glm::mat4 projView, wolf::RenderFilterType type, unsigned int postProcessingSharpTex, unsigned int postProcessingBlurTex, unsigned int depthTex, unsigned int depthTex2, std::string effect);
	void SetPercentGray(float p_percent);	

private:

	//Private Variables
	float percentGray = 0.0f;
	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_grayProgram;
	wolf::Program* g_blurProgram;
	wolf::Program* g_dofProgram;
	wolf::Program* g_dProgram;
	wolf::Vertex planeVertices[6] = {
		{-1.0f, 0.0f, -1.0f, 0, 1, 0, 1, 0.0f, 0.0f},
		{1.0f, 0.0f, -1.0f, 0, 1, 0, 1, 1.0f,  0.0f},
		{-1.0f, 0.0f, 1.0f, 0, 1, 0, 1, 0.0f, 1.0f},

		{ -1.0f, 0.0f, 1.0f, 0, 1, 0, 1, 0.0f,  1.0f},
		{ 1.0f, 0.0f, -1.0f, 0, 1, 0, 1, 1.0f,  0.0f},
		{ 1.0f, 0.0f, 1.0f, 0, 1, 0, 1,  1.0f, 1.0f}
	};

};

#endif
