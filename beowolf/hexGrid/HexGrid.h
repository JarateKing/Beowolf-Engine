#ifndef HEXGRID_H
#define HEXGRID_H

#include <iostream>
#include <vector>
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"

class HexGrid
{
public:
	HexGrid(int width, int length, float tileWidth);
	~HexGrid();
	void PrintOutLoc();
	void Render(glm::mat4 projView);

private:
	void GenerateLoc(int width, int length, float tileWidth);
	void GenerateHeights(int width, int length);
	void GenerateVerts(float tileWidth, float toEdge);
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