#include "HexSelector.h"
#include <iostream>
#include "W_ResourceLoader.h"

HexSelector::HexSelector(float tileWidth)
{
	tWidth = tileWidth;
	GenerateVerts();

	// set up rendering
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("cube"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(p_verts, sizeof(wolf::Vertex) * 36);

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

HexSelector::~HexSelector()
{
	delete g_pDecl;
	wolf::BufferManager::DestroyBuffer(g_pVB);
	//wolf::ProgramManager::DestroyProgram(g_dProgram);
}

void HexSelector::Update(int target, glm::vec2 position, float height)
{
	float diffX;
	float diffY;

	if (!(currTar == target) && target != -1)
	{
		currTar = target;
		currPos = position;
		currHeight = height;
	}
}

void HexSelector::Render(glm::mat4 projView)
{
	g_dProgram->Bind();

	// Bind Uniforms
	g_dProgram->SetUniform("projection", projView);
	g_dProgram->SetUniform("world", glm::translate(glm::vec3(currPos.x, currHeight + 0.01f, currPos.y)));
	g_dProgram->SetUniform("view", glm::mat4());

	// Set up source data
	g_pDecl->Bind();

	// Draw!
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void HexSelector::GenerateVerts()
{
	float toEdge = sqrt(pow(tWidth / 2, 2) - pow(tWidth / 4, 2));
	float diff = tWidth / 10.0f;
	float tWidth2 = tWidth - diff;
	float toEdgeSecond = sqrt(pow(tWidth2 / 2, 2) - pow(tWidth2 / 4, 2));
	glm::vec3 inColor{0, 0, 0};
	glm::vec3 outColor{255, 255, 255};

	wolf::Vertex v0 = {0.0f, 0.0f, (0.0f - tWidth / 2), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v1 = { (0.0f - toEdge), 0.0f, (0.0f - tWidth / 4), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v2 = { (0.0f - toEdge), 0.0f, (0.0f + tWidth / 4),inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v3 = { 0.0f, 0.0f, (0.0f + tWidth / 2), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v4 = { (0.0f + toEdge), 0.0f, (0.0f + tWidth / 4), inColor.x, inColor.y, inColor.z, 255};
	wolf::Vertex v5 = { (0.0f + toEdge), 0.0f, (0.0f - tWidth / 4), inColor.x, inColor.y, inColor.z, 255};

	wolf::Vertex v6 = { 0.0f, 0.0f, (0.0f - tWidth2 / 2), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v7 = { (0.0f - toEdgeSecond), 0.0f, (0.0f - tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v8 = { (0.0f - toEdgeSecond), 0.0f, (0.0f + tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v9 = { 0.0f, 0.0f, (0.0f + tWidth2 / 2) , outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v10 = { (0.0f + toEdgeSecond), 0.0f, (0.0f + tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};
	wolf::Vertex v11 = { (0.0f + toEdgeSecond), 0.0f, (0.0f - tWidth2 / 4), outColor.x, outColor.y, outColor.z, 255};

	p_verts[0] = v0;
	p_verts[1] = v1;
	p_verts[2] = v6;
	p_verts[3] = v6;
	p_verts[4] = v7;
	p_verts[5] = v1;

	p_verts[6] = v1;
	p_verts[7] = v2;
	p_verts[8] = v7;
	p_verts[9] = v2;
	p_verts[10] = v7;
	p_verts[11] = v8;

	p_verts[12] = v2;
	p_verts[13] = v3;
	p_verts[14] = v8;
	p_verts[15] = v3;
	p_verts[16] = v8;
	p_verts[17] = v9;

	p_verts[18] = v3;
	p_verts[19] = v4;
	p_verts[20] = v9;
	p_verts[21] = v4;
	p_verts[22] = v9;
	p_verts[23] = v10;

	p_verts[24] = v4;
	p_verts[25] = v5;
	p_verts[26] = v10;
	p_verts[27] = v5;
	p_verts[28] = v10;
	p_verts[29] = v11;

	p_verts[30] = v5;
	p_verts[31] = v0;
	p_verts[32] = v11;
	p_verts[33] = v0;
	p_verts[34] = v6;
	p_verts[35] = v11;
}
