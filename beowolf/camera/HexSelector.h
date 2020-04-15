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
	//Public Methods
	HexSelector(float tileWidth);
	~HexSelector();
	void Update(int target, glm::vec2 position, float height);
	void Render(glm::mat4 projView);

private:
	//Private Methods
	void GenerateVerts();
	
	//Private Variables
	int m_currTar;
	float m_tWidth;
	float m_currHeight = 0.0f;
	glm::vec2 m_currPos;
	wolf::Vertex m_verts[36];
	wolf::VertexBuffer* m_pVB;
	wolf::VertexDeclaration* m_pDecl;
	wolf::Program* m_dProgram;
};

#endif