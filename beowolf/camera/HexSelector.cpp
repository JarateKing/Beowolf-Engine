#include "HexSelector.h"
#include <iostream>
#include "W_ResourceLoader.h"

HexSelector::HexSelector(float tileWidth)
{
	//Set the Selectors width
	m_tWidth = tileWidth;
	GenerateVerts();

	// set up rendering
	m_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("cube"));
	m_pVB = wolf::BufferManager::CreateVertexBuffer(m_verts, sizeof(wolf::Vertex) * 36);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	wolf::Vertex::applyAttributes(m_pDecl);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();
}

//Deconstructor
HexSelector::~HexSelector()
{
	delete m_pDecl;
	wolf::BufferManager::DestroyBuffer(m_pVB);
	//wolf::ProgramManager::DestroyProgram(g_dProgram);
}

//Update
void HexSelector::Update(int target, glm::vec2 position, float height)
{
	float diffX;
	float diffY;

	//Check if the current target is different
	if (!(m_currTar == target) && target != -1)
	{
		m_currTar = target;
		m_currPos = position;
		m_currHeight = height;
	}
}

//Render
void HexSelector::Render(glm::mat4 projView)
{
	m_dProgram->Bind();

	// Bind Uniforms
	m_dProgram->SetUniform("projection", projView);
	m_dProgram->SetUniform("world", glm::translate(glm::vec3(m_currPos.x, m_currHeight + 0.01f, m_currPos.y)));
	m_dProgram->SetUniform("view", glm::mat4());

	// Set up source data
	m_pDecl->Bind();

	// Draw!
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

//Generate Vertices for Selector
void HexSelector::GenerateVerts()
{
	float toEdge = sqrt(pow(m_tWidth / 2, 2) - pow(m_tWidth / 4, 2));
	float diff = m_tWidth / 10.0f;
	float tWidth2 = m_tWidth - diff;
	float toEdgeSecond = sqrt(pow(tWidth2 / 2, 2) - pow(tWidth2 / 4, 2));
	glm::vec3 inColor{0, 0, 0};
	glm::vec3 outColor{255, 255, 255};
	
	//Inside Verts
	wolf::Vertex v0 = {0.0f, 0.0f, (0.0f - m_tWidth / 2), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v1 = { (0.0f - toEdge), 0.0f, (0.0f - m_tWidth / 4), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v2 = { (0.0f - toEdge), 0.0f, (0.0f + m_tWidth / 4),inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v3 = { 0.0f, 0.0f, (0.0f + m_tWidth / 2), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v4 = { (0.0f + toEdge), 0.0f, (0.0f + m_tWidth / 4), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v5 = { (0.0f + toEdge), 0.0f, (0.0f - m_tWidth / 4), inColor.x, inColor.y, inColor.z, 255};

	//Outside Verts
	wolf::Vertex v6 = { 0.0f, 0.0f, (0.0f - tWidth2 / 2), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v7 = { (0.0f - toEdgeSecond), 0.0f, (0.0f - tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v8 = { (0.0f - toEdgeSecond), 0.0f, (0.0f + tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v9 = { 0.0f, 0.0f, (0.0f + tWidth2 / 2) , outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v10 = { (0.0f + toEdgeSecond), 0.0f, (0.0f + tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v11 = { (0.0f + toEdgeSecond), 0.0f, (0.0f - tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};

	m_verts[0] = v0;
	m_verts[1] = v1;
	m_verts[2] = v6;
	m_verts[3] = v6;
	m_verts[4] = v7;
	m_verts[5] = v1;

	m_verts[6] = v1;
	m_verts[7] = v2;
	m_verts[8] = v7;
	m_verts[9] = v2;
	m_verts[10] = v7;
	m_verts[11] = v8;

	m_verts[12] = v2;
	m_verts[13] = v3;
	m_verts[14] = v8;
	m_verts[15] = v3;
	m_verts[16] = v8;
	m_verts[17] = v9;

	m_verts[18] = v3;
	m_verts[19] = v4;
	m_verts[20] = v9;
	m_verts[21] = v4;
	m_verts[22] = v9;
	m_verts[23] = v10;

	m_verts[24] = v4;
	m_verts[25] = v5;
	m_verts[26] = v10;
	m_verts[27] = v5;
	m_verts[28] = v10;
	m_verts[29] = v11;

	m_verts[30] = v5;
	m_verts[31] = v0;
	m_verts[32] = v11;
	m_verts[33] = v0;
	m_verts[34] = v6;
	m_verts[35] = v11;
}
