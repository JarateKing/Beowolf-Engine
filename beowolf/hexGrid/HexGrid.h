#ifndef HEXGRID_H
#define HEXGRID_H

#include <iostream>
#include <vector>
#include "BMWModel.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_Texture.h"
#include "W_TextureManager.h"
#include "AIPathfinder.h"
#include "camera/HexSelector.h"
#include "Effect.h"

class HexGrid
{
public:
	HexGrid(int width, int length, float tileWidth, float minHeight, float maxHeight, std::string texFile, std::string savedata = "");
	~HexGrid();
	void PrintOutLoc();
	void Render(glm::mat4 p_view, glm::mat4 p_proj, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTexture, float minHeight, float maxHeight);
	std::vector<float> GetHeights();
	std::vector<glm::vec2> GetPos();
	int GetSize();
	void Update(int target, float delta);
	std::vector<int> GetPathway(int startTarget, int endTarget);
	bool cmpf(float a, float b);
	int GetRandomBorder();
	bool isMountain(int pos);
	bool isDesert(int pos);
	void BlockNodePositions(glm::vec3 p_nodePos);
	void ClearBlocks();
	void SetLightDir(glm::vec3 dir);
	void SetDiffuse(glm::vec4 dif);
	void SetAmbient(glm::vec4 amb);
	void StartTargeting(int target, int max);
	void StopTargeting();

private:
	void GenerateLoc(int width, int length, float tileWidth, bool partiallyPregenerated = false);
	void GenerateHeights(int width, int length, float minHeight, float maxHeight);
	void GenerateVerts(float tileWidth, float toEdge);
	void SmoothFullHeights(int width, int numTimes);
	void GroupTextures(int width, bool partiallyPregenerated = false);
	void GenerateHexJSON(int width, int length, float tileWidth);
	bool WithinSameLine(int tile1, int tile2, int width);
	bool WithinLineBelow(int tile1, int tile2, int width);
	bool WithinBounds(int tile);
	bool isBorder(int tile);

	float minH, maxH;

	week9::AIPathfinder* pathFinder;
	wolf::Texture* pTex;
	wolf::Texture* pTex2;
	wolf::BMWModel* test;
	std::vector<wolf::BMWModel*> trees;
	std::vector<wolf::BMWModel*> rocks;
	std::vector<HexSelector*> selections;
	std::vector<int> selectionTabs;

	std::vector<glm::vec3> rockPos;
	std::vector<glm::mat4> rockTRS;

	std::vector<int> roads;
	std::vector<int> mountains;
	std::vector<int> desert;
	std::vector<int> grass;

	std::vector<int> workables;

	std::vector<glm::vec2> positions;
	std::vector<float> heights;

	wolf::Vertex verts[12];
	wolf::Vertex p_verts[4000000];
	std::vector<wolf::Vertex> vertices;

	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_dProgram;
	wolf::Program* g_dShadowProgram;
	
	glm::vec4 ambLight = glm::vec4(1, 1, 1, 1);
	glm::vec4 difLight = glm::vec4(1, 1, 1, 1);
	glm::vec3 lightDir = glm::vec3(1, 1, 1);

	std::vector<Effect*> m_particleEffects;
	std::vector<Effect*> m_particleEffectsNoBillboard;
	glm::mat4 m_particleProjMatrix;
	glm::mat4 m_particleProjMatrixNoBillboard;

	bool targeting = false;
	bool changed = true;
	int targetingT = -1;
	int abstractTarget = -1;
	int lastFrame = -1;
	int targetingMax;

	int m_width;
	int m_height;
};

#endif