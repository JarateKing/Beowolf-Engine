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
	//Public Methods
	HexGrid(const int width, const int length, const float tileWidth, const float minHeight, const float maxHeight, const std::string texFile, const std::string savedata = "");
	~HexGrid();
	void PrintOutLoc();
	void Render(glm::mat4 p_view, glm::mat4 p_proj, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTexture, float minHeight, float maxHeight);
	std::vector<float> GetHeights() const;
	std::vector<glm::vec2> GetPos() const;
	int GetSize() const;
	void Update(int target, float delta);
	std::vector<int> GetPathway(int startTarget, int endTarget);
	bool cmpf(float a, float b) const;
	int GetRandomBorder() const;
	bool isMountain(int pos) const;
	bool isDesert(int pos) const;
	void BlockNodePositions(glm::vec3 p_nodePos) const;
	void ClearBlocks();
	void SetLightDir(glm::vec3 dir);
	void SetDiffuse(glm::vec4 dif);
	void SetAmbient(glm::vec4 amb);
	void StartTargeting(int target, int max);
	void StopTargeting();

private:
	//Private Methods
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

	//Private Variables
	week9::AIPathfinder* m_pathFinder;
	wolf::Texture* m_pTex;
	wolf::Texture* m_pTex2;
	wolf::BMWModel* m_test;

	std::vector<wolf::BMWModel*> m_trees;
	std::vector<wolf::BMWModel*> m_rocks;
	std::vector<HexSelector*> m_selections;
	std::vector<int> m_selectionTabs;
	std::vector<glm::vec3> m_rockPos;
	std::vector<glm::mat4> m_rockTRS;
	std::vector<int> m_roads;
	std::vector<int> m_mountains;
	std::vector<int> m_desert;
	std::vector<int> m_grass;
	std::vector<int> m_workables;
	std::vector<glm::vec2> m_positions;
	std::vector<float> m_heights;
	std::vector<wolf::Vertex> m_vertices;

	wolf::Vertex m_verts[12];
	wolf::Vertex p_verts[4000000];
	wolf::VertexBuffer* m_pVB;
	wolf::VertexDeclaration* m_pDecl;
	wolf::Program* m_dProgram;
	wolf::Program* m_dShadowProgram;
	
	glm::vec4 m_ambLight = glm::vec4(1, 1, 1, 1);
	glm::vec4 m_difLight = glm::vec4(1, 1, 1, 1);
	glm::vec3 m_lightDir = glm::vec3(1, 1, 1);

	std::vector<Effect*> m_particleEffects;
	std::vector<Effect*> m_particleEffectsNoBillboard;
	glm::mat4 m_particleProjMatrix;
	glm::mat4 m_particleProjMatrixNoBillboard;

	bool m_targeting = false;
	bool m_changed = true;
	int m_targetingT = -1;
	int m_abstractTarget = -1;
	int m_lastFrame = -1;
	int m_targetingMax;
	int m_width;
	int m_height;
	float m_minH, m_maxH;
};

#endif