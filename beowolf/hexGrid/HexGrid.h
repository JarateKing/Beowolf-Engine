#ifndef HEXGRID_H
#define HEXGRID_H

#include <iostream>
#include <vector>
#include <BMWModel.h>
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_Texture.h"
#include "W_TextureManager.h"

class HexGrid
{
public:
	HexGrid(int width, int length, float tileWidth, float minHeight, float maxHeight, std::string texFile);
	~HexGrid();
	void PrintOutLoc();
	void Render(glm::mat4 projView);
	std::vector<float> GetHeights();
	std::vector<glm::vec2> GetPos();

private:
	void GenerateLoc(int width, int length, float tileWidth);
	void GenerateHeights(int width, int length, float minHeight, float maxHeight);
	void GenerateVerts(float tileWidth, float toEdge);
	void SmoothFullHeights(int width, int numTimes);
	void GroupTextures(int width);

	float minH, maxH;

	wolf::Texture* pTex;
	wolf::BMWModel* test;
	std::vector<wolf::BMWModel*> trees;

	std::vector<int> roads;
	std::vector<int> mountains;
	std::vector<int> desert;
	std::vector<int> grass;

	std::vector<glm::vec2> positions;
	std::vector<float> heights;

	wolf::Vertex verts[12];
	wolf::Vertex p_verts[4000000];
	std::vector<wolf::Vertex> vertices;

	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_dProgram;
};

#endif