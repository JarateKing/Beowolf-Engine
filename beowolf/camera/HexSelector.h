#ifndef HEXSELECTOR_H
#define HEXSELECTOR_H

#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_Texture.h"
#include "W_TextureManager.h"

class HexSelector
{
public:
	HexSelector(float tileWidth);
	~HexSelector();
	void Update(int target, glm::vec2 position, float height);
	void Render(glm::mat4 projView);

private:
	void GenerateVerts();
	
	float tWidth;
	glm::vec2 currPos;
	int currTar;
	float currHeight = 0.0f;
	wolf::Vertex p_verts[36];
	
	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_dProgram;
};

#endif