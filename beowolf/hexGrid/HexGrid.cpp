#include "HexGrid.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"
#include "W_Input.h"
#include <fstream>
#include <string>
#include <sstream>
#define EPSILON_VALUE 0.01


//Initialize HexGrid Variables and Load Data if Needed
HexGrid::HexGrid(const int width, const int length, const float tileWidth, const float minHeight, const float maxHeight, const std::string texFile, const std::string savedata)
{
	m_width = width;
	m_height = length;

	m_pathFinder->CreateInstance();
	m_minH = minHeight;
	m_maxH = maxHeight;
	
	if (savedata == "") {
		GenerateHeights(width, length, minHeight, maxHeight);
		SmoothFullHeights(width, 3);
		GenerateLoc(width, length, tileWidth);
	}
	else {
		std::ifstream jsonIn(savedata);
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		json savejson = json::parse(jsonFileData);

		for (auto tile : savejson["Grid"]) {
			int id = tile["id"];
			bool isDesert = tile["desert"];
			bool isMountain = tile["mountain"];

			m_heights.push_back(tile["height"]);

			if (isDesert)
				m_desert.push_back(id);
			if (isMountain)
				m_mountains.push_back(id);
		}

		GenerateLoc(width, length, tileWidth, true);
	}
	GenerateHexJSON(width, length, tileWidth);
	m_pathFinder->Instance()->Load("resources/objects/AIPathfindingDataTest.json");

	//Set Max Trees and Rocks in Map
	int rockInWaterMax = 30;
	int rockAlongShoreMax = 15;

	int treeMaxNum = (m_positions.size() - m_mountains.size() - m_desert.size() - m_roads.size()) / 6 - 1;
	int treeNum = treeMaxNum;
	if (treeMaxNum > 4)
		treeNum = wolf::RNG::GetRandom(2, treeMaxNum);
	std::vector<int> treePos;
	for (int i = 0; i < treeNum; i++)
	{
		bool passed = false;
		while (!passed)
		{
			int next = wolf::RNG::GetRandom(0, m_positions.size());
			if (std::find(treePos.begin(), treePos.end(), next) == treePos.end() && std::find(m_grass.begin(), m_grass.end(), next) != m_grass.end())
			{
				treePos.push_back(next);
				passed = true;
			}
		}
	}

	//Initialize Tree and Rock Models
	int rnd;
	std::string model;
	for (int i = 0; i < treeNum; i++)
	{
		auto shaders = wolf::ResourceLoader::Instance().getShaders("lit_textured");
		auto shadowShaders = wolf::ResourceLoader::Instance().getShaders("shadow_map");
		rnd = wolf::RNG::GetRandom(0, 3);
		switch (rnd) {

		case 0:
			model = "Fir_Tree.bmw";
			break;
		case 1:
			model = "Oak_Tree.bmw";
			break;
		case 2:
			model = "Palm_Tree.bmw";
			break;
		case 3:
			model = "Poplar_Tree.bmw";
			break;
		}

		m_test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(model), shaders.first, shaders.second, shadowShaders.first, shadowShaders.second);
		float setScale = wolf::RNG::GetRandom(0.01f, 0.02f);
		m_test->setTransform(glm::translate(glm::vec3(m_positions.at(treePos.at(i)).x, m_heights.at(treePos.at(i)), m_positions.at(treePos.at(i)).y)) * glm::rotate(180.0f, glm::vec3(0, wolf::RNG::GetRandom(0.0f, 5.0f), 0)) * glm::scale(glm::vec3(setScale, setScale, setScale)));
		m_test->setLightAmbient(glm::vec4(0.999f, 0.999f, 0.899f, 1.0f));
		m_test->setLightDiffuse(glm::vec4(0.988f, 1.0f, 0.788f, 1.0f));
		m_test->setLightDir(glm::vec3(90.0f, 90.0f, 90.0f));
		m_trees.push_back(m_test);
	}

	float scaleX;
	float xPos = 0.0f;
	float zPos = 0.0f;

	for (int i = 0; i < rockInWaterMax; i++)
	{

		xPos = wolf::RNG::GetRandom(-100.0f, 100.0f);
		zPos = wolf::RNG::GetRandom(-100.0f, 100.0f);
		scaleX = wolf::RNG::GetRandom(0.5f, 4.0f);

		while (xPos <= 40.0f && xPos >= -40.0f && zPos <= 40.0f && zPos >= -40.0f)
		{
			xPos = wolf::RNG::GetRandom(-75.0f, 75.0f);
			zPos = wolf::RNG::GetRandom(-75.0f, 75.0f);
		}

		m_rockTRS.push_back(glm::translate(xPos, 5.0f, zPos) * glm::rotate(90.0f, glm::vec3(0, 0, 1)) * glm::rotate(90.0f, glm::vec3(0, wolf::RNG::GetRandom(0.0f, 5.0f), 0)) * glm::scale(glm::vec3(scaleX, scaleX, scaleX)));
	}

	std::vector<int> tilesWithRocks;
	std::vector<int>::iterator it;

	for (int i = 0; i < rockAlongShoreMax; i++)
	{
		int curPlacement = GetRandomBorder();
		it = find(tilesWithRocks.begin(), tilesWithRocks.end(), curPlacement);
		while (it != tilesWithRocks.end())
		{
			curPlacement = GetRandomBorder();
			it = find(tilesWithRocks.begin(), tilesWithRocks.end(), curPlacement);
		}
		tilesWithRocks.push_back(curPlacement);
	}
	
	for (int i = 0; i < rockAlongShoreMax; i++)
	{
		scaleX = wolf::RNG::GetRandom(1.0f, 2.5f);

		if (m_positions.at(tilesWithRocks.at(i)).x <= 0 && m_positions.at(tilesWithRocks.at(i)).y <= 0)
		{
			m_rockTRS.push_back(glm::translate(glm::vec3(m_positions.at(tilesWithRocks.at(i)).x - wolf::RNG::GetRandom(2.5f,5.0f), 5.0f, m_positions.at(tilesWithRocks.at(i)).y - wolf::RNG::GetRandom(2.5f, 5.0f))) * glm::rotate(90.0f, glm::vec3(0, 0, 1)) * glm::rotate(90.0f, glm::vec3(0, wolf::RNG::GetRandom(0.0f, 5.0f), 0)) * glm::scale(glm::vec3(scaleX, scaleX, scaleX)));
		}
		else if (m_positions.at(tilesWithRocks.at(i)).x <= 0 && m_positions.at(tilesWithRocks.at(i)).y >= 0)
		{
			m_rockTRS.push_back(glm::translate(glm::vec3(m_positions.at(tilesWithRocks.at(i)).x - wolf::RNG::GetRandom(2.5f, 5.0f), 5.0f, m_positions.at(tilesWithRocks.at(i)).y + wolf::RNG::GetRandom(2.5f, 5.0f))) * glm::rotate(90.0f, glm::vec3(0, 0, 1)) * glm::rotate(90.0f, glm::vec3(0, wolf::RNG::GetRandom(0.0f, 5.0f), 0)) * glm::scale(glm::vec3(scaleX, scaleX, scaleX)));
		}
		else if (m_positions.at(tilesWithRocks.at(i)).x >= 0 && m_positions.at(tilesWithRocks.at(i)).y >= 0)
		{
			m_rockTRS.push_back(glm::translate(glm::vec3(m_positions.at(tilesWithRocks.at(i)).x + wolf::RNG::GetRandom(2.5f, 5.0f), 5.0f, m_positions.at(tilesWithRocks.at(i)).y + wolf::RNG::GetRandom(2.5f, 5.0f))) * glm::rotate(90.0f, glm::vec3(0, 0, 1)) * glm::rotate(90.0f, glm::vec3(0, wolf::RNG::GetRandom(0.0f, 5.0f), 0)) * glm::scale(glm::vec3(scaleX, scaleX, scaleX)));
		}
		else if (m_positions.at(tilesWithRocks.at(i)).x >= 0 && m_positions.at(tilesWithRocks.at(i)).y <= 0)
		{
			m_rockTRS.push_back(glm::translate(glm::vec3(m_positions.at(tilesWithRocks.at(i)).x + wolf::RNG::GetRandom(2.5f, 5.0f), 5.0f, m_positions.at(tilesWithRocks.at(i)).y - wolf::RNG::GetRandom(2.5f, 5.0f))) * glm::rotate(90.0f, glm::vec3(0, 0, 1)) * glm::rotate(90.0f, glm::vec3(0, wolf::RNG::GetRandom(0.0f, 5.0f), 0)) * glm::scale(glm::vec3(scaleX, scaleX, scaleX)));
		}
	}

	auto shaders = wolf::ResourceLoader::Instance().getShaders("lit_textured_instanced");
	auto shadowShaders = wolf::ResourceLoader::Instance().getShaders("shadow_map_instanced");

	m_test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("rock.bmw"), shaders.first, shaders.second, shadowShaders.first, shadowShaders.second);
	float setScale = wolf::RNG::GetRandom(1.0f, 2.0f);
	m_test->setInstancedVariable(m_rockTRS);
	m_test->setLightAmbient(glm::vec4(0.999f, 0.999f, 0.899f, 1.0f));
	m_test->setLightDiffuse(glm::vec4(0.988f, 1.0f, 0.788f, 1.0f));
	m_test->setLightDir(glm::vec3(90.0f, 90.0f, 90.0f));
	m_rocks.push_back(m_test);

	// set up rendering
	m_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("hex"));
	m_dShadowProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("shadow_map"));
	m_pVB = wolf::BufferManager::CreateVertexBuffer(p_verts, sizeof(wolf::Vertex) * m_vertices.size());

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	wolf::Vertex::applyAttributes(m_pDecl);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();

	m_pTex = wolf::TextureManager::CreateTexture(texFile);
	m_pTex->SetFilterMode(wolf::Texture::FM_LinearMipmap, wolf::Texture::FM_Linear);
	m_pTex->SetWrapMode(wolf::Texture::WM_Repeat);
	std::vector<glm::vec3> dPos;
	for (int i = 0; i < m_desert.size(); i++)
	{
		if(m_desert.at(i) < m_positions.size())
			dPos.push_back(glm::vec3(m_positions.at(m_desert.at(i)).x, 0.0f, m_positions.at(m_desert.at(i)).y));
	}
	m_pathFinder->Instance()->SetDesertPositions(dPos);

	// add particle effects
	for (int i = 0; i < 3; i++) {
		int pos = m_grass.at(wolf::RNG::GetRandom(0, m_grass.size() - 1));
		m_particleEffects.push_back(new Effect("resources/particles/butterfly.json"));
		m_particleEffects[m_particleEffects.size() - 1]->SetPos(glm::vec3(m_positions.at(pos).x, m_heights.at(pos), m_positions.at(pos).y));
	}
}

//Deconstructor
HexGrid::~HexGrid()
{
	wolf::BufferManager::DestroyBuffer(m_pVB);
	wolf::ProgramManager::DestroyProgram(m_dProgram);
	wolf::ProgramManager::DestroyProgram(m_dShadowProgram);
	m_pathFinder->DestroyInstance();

	if(m_pTex != NULL)
		wolf::TextureManager().DestroyTexture(m_pTex);

	if(m_pTex2 != NULL)
		wolf::TextureManager().DestroyTexture(m_pTex2);
	delete m_test;

	for (auto it : m_selections){
		delete it;
	}
	m_selections.clear();

	for (auto it : m_particleEffects) {
		delete it;
	}
	m_selections.clear();

	for (auto it : m_particleEffectsNoBillboard) {
		delete it;
	}
	m_selections.clear();
}

//Generate the Heights for each Hex Tile Randomly
void HexGrid::GenerateHeights(int width, int length, float minHeight, float maxHeight)
{
	int numWide;
	int numShort;

	if (length % 2 == 0)
	{
		numWide = length / 2;
	}
	else
	{
		numWide = (length / 2) + 1;
	}

	numShort = length - numWide;

	int numPos = (numWide * width) + (numShort * (width - 1));

	for (int i = 0; i < numPos; i++)
	{
		m_heights.push_back(wolf::RNG::GetRandom(minHeight, maxHeight));
	}
}

// Returns a Random Border Tile Used for Taking out Edge Tiles
int HexGrid::GetRandomBorder() const {
	int side = wolf::RNG::GetRandom(0, 3);

	if (side == 0) {
		return wolf::RNG::GetRandom(0, m_width - 1);
	}
	else if (side == 1) {
		return m_positions.size() - wolf::RNG::GetRandom(1, m_width);
	}
	else if (side == 2) {
		int zpos = wolf::RNG::GetRandom(1, m_height - 2);

		int sum = 0;
		for (int i = 0; i < zpos; i++) {
			sum += m_width - (i % 2);
		}
		return sum;
	}
	else if (side == 3) {
		int zpos = wolf::RNG::GetRandom(2, m_height - 1);

		int sum = 0;
		for (int i = 0; i < zpos; i++) {
			sum += m_width - (i % 2);
		}
		return sum - 1;
	}
}

//Generate Tiles
void HexGrid::GenerateLoc(int width, int length, float tileWidth, bool partiallyPregenerated)
{
	float toEdge = sqrt((pow((tileWidth/2), 2)) - (pow((tileWidth/4), 2)));
	float startX = ((width * (toEdge * 2)) / 2) - (width * (toEdge * 2)) + toEdge;
	float diffX = toEdge * 2;
	float diffY = ((tileWidth / 4) * 3);

	int numWide;
	int numShort;
	int numTotal;

	if (length % 2 == 0)
	{
		numWide = length / 2;
	}
	else
	{
		numWide = (length / 2) + 1;
	}

	numShort = length - numWide;
	numTotal = (numWide * width) + (numShort *(width - 1));
	float startY = ((((tileWidth * numWide) + ((tileWidth / 2) * numShort)) / 2) - ((tileWidth * numWide) + ((tileWidth / 2) * numShort))) + (tileWidth / 2);

	int k = 0;
	for (int i = 0; i < length; i++)
	{
		if (k == 0)
		{
			for (int j = 0; j < width; j++)
			{
				m_positions.push_back(glm::vec2((startX + (diffX * j)), (startY + (diffY * i))));
			}			
			k = 1;
		}
		else
		{
			for (int j = 0; j < (width - 1); j++)
			{
				m_positions.push_back(glm::vec2(((startX + toEdge) + (diffX * j)), (startY + (diffY * i))));
			}
			k = 0;
		}
	}
	GroupTextures(width, partiallyPregenerated);
	GenerateVerts(tileWidth, toEdge);
}

//Debug Method for Printing out Locations
void HexGrid::PrintOutLoc()
{
	for (int i = 0; i < m_heights.size(); i++)
	{
		std::cout << m_heights.at(i) << std::endl;
	}
}

//Method to Generate Vertices for HexGrid based on Positions and Heights and Types
void HexGrid::GenerateVerts(float tileWidth, float toEdge)
{
	for (int i = 0; i < m_positions.size(); i++)
	{
		if (!(std::find(m_mountains.begin(), m_mountains.end(), i) != m_mountains.end() && isBorder(i)))
		{
			int ranTexX = wolf::RNG::GetRandom(0, 1);
			int ranTexY = wolf::RNG::GetRandom(0, 1);
			float xNormal = 0.894427f; //0.25f;
			float yNormal = -0.447214f; //0.125f;

			glm::vec2 p0Tex(0.25f + (0.5f * ranTexX), 0.0f + (0.5f * ranTexY));
			glm::vec2 p1Tex(0.0f + (0.5f * ranTexX), 0.125f + (0.5f * ranTexY));
			glm::vec2 p2Tex(0.0f + (0.5f * ranTexX), 0.375f + (0.5f * ranTexY));
			glm::vec2 p3Tex(0.25f + (0.5f * ranTexX), 0.5f + (0.5f * ranTexY));
			glm::vec2 p4Tex(0.5f + (0.5f * ranTexX), 0.375f + (0.5f * ranTexY));
			glm::vec2 p5Tex(0.5f + (0.5f * ranTexX), 0.125f + (0.5f * ranTexY));

			if (std::find(m_roads.begin(), m_roads.end(), i) != m_roads.end())
			{
				p0Tex.x = 0.25f;
				p0Tex.y = 0.5f;
				p1Tex.x = 0.0f;
				p1Tex.y = 0.625f;
				p2Tex.x = 0.0f;
				p2Tex.y = 0.875f;
				p3Tex.x = 0.25f;
				p3Tex.y = 1.0f;
				p4Tex.x = 0.5f;
				p4Tex.y = 0.875f;
				p5Tex.x = 0.5f;
				p5Tex.y = 0.625f;
			}
			else if (std::find(m_mountains.begin(), m_mountains.end(), i) != m_mountains.end())
			{
				p0Tex.x = 0.75f;
				p0Tex.y = 0.0f;
				p1Tex.x = 0.5f;
				p1Tex.y = 0.125f;
				p2Tex.x = 0.5f;
				p2Tex.y = 0.375f;
				p3Tex.x = 0.75f;
				p3Tex.y = 0.5f;
				p4Tex.x = 1.0f;
				p4Tex.y = 0.375f;
				p5Tex.x = 1.0f;
				p5Tex.y = 0.125f;
			}
			else if (std::find(m_desert.begin(), m_desert.end(), i) != m_desert.end())
			{
				p0Tex.x = 0.75f;
				p0Tex.y = 0.5f;
				p1Tex.x = 0.5f;
				p1Tex.y = 0.625f;
				p2Tex.x = 0.5f;
				p2Tex.y = 0.875f;
				p3Tex.x = 0.75f;
				p3Tex.y = 1.0f;
				p4Tex.x = 1.0f;
				p4Tex.y = 0.875f;
				p5Tex.x = 1.0f;
				p5Tex.y = 0.625f;
			}
			else
			{
				p0Tex.x = 0.25f;
				p0Tex.y = 0.0f;
				p1Tex.x = 0.0f;
				p1Tex.y = 0.125f;
				p2Tex.x = 0.0f;
				p2Tex.y = 0.375f;
				p3Tex.x = 0.25f;
				p3Tex.y = 0.5f;
				p4Tex.x = 0.5f;
				p4Tex.y = 0.375f;
				p5Tex.x = 0.5f;
				p5Tex.y = 0.125f;
			}

		//Inner Top Verts
		m_verts[0] = { m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y + tileWidth / 2), 255, 255, 255, 255, p0Tex.x, p0Tex.y, 0.0f, 1.0f, 0.0f };
		m_verts[1] = { (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 255, 255, 255, 255, p1Tex.x, p1Tex.y, 0.0f, 1.0f, 0.0f };
		m_verts[2] = { (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 255, 255, 255, 255, p2Tex.x, p2Tex.y, 0.0f, 1.0f, 0.0f };
		m_verts[3] = { m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y - tileWidth / 2), 255, 255, 255, 255, p3Tex.x, p3Tex.y, 0.0f, 1.0f, 0.0f };
		m_verts[4] = { (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 255, 255, 255, 255, p4Tex.x, p4Tex.y, 0.0f, 1.0f, 0.0f };
		m_verts[5] = { (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 255, 255, 255, 255, p5Tex.x, p5Tex.y, 0.0f, 1.0f, 0.0f };

		//Bottom Verts
		m_verts[6] = { m_positions.at(i).x, -0.0f, (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f };
		m_verts[7] = { (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };
		m_verts[8] = { (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };
		m_verts[9] = { m_positions.at(i).x, 0.0f, (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f };
		m_verts[10] = { (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };
		m_verts[11] = { (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };

		//Top Vertices
		m_vertices.push_back(m_verts[3]);
		m_vertices.push_back(m_verts[4]);
		m_vertices.push_back(m_verts[5]);
		m_vertices.push_back(m_verts[3]);
		m_vertices.push_back(m_verts[5]);
		m_vertices.push_back(m_verts[0]);
		m_vertices.push_back(m_verts[3]);
		m_vertices.push_back(m_verts[1]);
		m_vertices.push_back(m_verts[0]);
		m_vertices.push_back(m_verts[3]);
		m_vertices.push_back(m_verts[2]);
		m_vertices.push_back(m_verts[1]);

		if (std::find(m_mountains.begin(), m_mountains.end(), i) != m_mountains.end())
		{
			//front left vertices
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y + tileWidth / 2), 200, 200, 200, 255, 0.6f, 0.45f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y + tileWidth / 2), 200, 200, 200, 255, 0.6f, 0.45f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.6f, 0.0f, xNormal, 0.0f, yNormal });

			//Left Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f });

			//Back left Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f, -xNormal, 0.0f, yNormal  });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 1.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y - tileWidth / 2), 200, 200, 200, 255, 1.0f, 0.45f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 1.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f, -xNormal, 0.0f, yNormal });

			//Back right Vertices
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y - tileWidth / 2), 200, 200, 200, 255, 1.0f, 0.45f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y - tileWidth / 2), 200, 200, 200, 255, 1.0f, 0.45f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 1.0f, 0.0f, -xNormal, 0.0f, -yNormal });

			//Right Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f });
			
			//Front right Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 200, 255, 1.0f, 0.45f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y + tileWidth / 2), 0, 0, 200, 255, 0.6f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y + tileWidth / 2), 0, 0, 200, 255, 0.6f, 0.45f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 200, 255, 1.0f, 0.45f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y + tileWidth / 2), 0, 0, 200, 255, 0.6f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 200, 255, 1.0f, 0.0f, xNormal, 0.0f, yNormal });
		}
		else
		{
			//Front left Vertices
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, xNormal, 0.0f, yNormal });
			

			//Left Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f });

			//Back left Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x - toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });

			//Back right Vertices
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, -yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, -yNormal });

			//Right Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });

			//Front right Vertices
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, m_heights.at(i), (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), m_heights.at(i), (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ m_positions.at(i).x, 0.0f, (m_positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			m_vertices.push_back(wolf::Vertex{ (m_positions.at(i).x + toEdge), 0.0f, (m_positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f, -xNormal, 0.0f, yNormal });
			}
		}
	}

	for (int i = 0; i < m_vertices.size(); i++)
	{
		p_verts[i] = m_vertices.at(i);
	}
}

//Method to Group Textures Together
void HexGrid::GroupTextures(int width, bool partiallyPregenerated)
{
	bool ranInto = false;
	int midPoint = (int)(m_positions.size()/2);
	m_roads.push_back(midPoint);

	int path;
	int lastTaken = -1;

	//Creating road layout
	while ((m_roads.size() < (int)(m_positions.size() * 0.35)))
	{
		int pos = midPoint;
		ranInto = false;

		while (!ranInto)
		{
			path = wolf::RNG::GetRandom(0, 5);

			while (path == lastTaken)
			{
				path = wolf::RNG::GetRandom(0, 5);
			}
			lastTaken = abs(path-3);

			if ((std::find(m_roads.begin(), m_roads.end(), (pos + (width - 1))) != m_roads.end() || (pos + (width - 1)) >= m_positions.size())  &&
				(std::find(m_roads.begin(), m_roads.end(), (pos - 1)) != m_roads.end() || (pos - 1) <= 0) &&
				(std::find(m_roads.begin(), m_roads.end(), (pos - width)) != m_roads.end() || (pos - width) <= 0) &&
				(std::find(m_roads.begin(), m_roads.end(), (pos - (width - 1))) != m_roads.end() || (pos - (width - 1)) <= 0) &&
				(std::find(m_roads.begin(), m_roads.end(), (pos + 1)) != m_roads.end() || (pos + 1) >= m_positions.size()) &&
				(std::find(m_roads.begin(), m_roads.end(), (pos + width)) != m_roads.end() || (pos + width) >= m_positions.size()))
			{
				midPoint = wolf::RNG::GetRandom(0, m_positions.size());
			}

			switch (path)
			{
				case 0:
					if ((m_roads.size() > (m_positions.size() * 0.35)) || std::find(m_roads.begin(), m_roads.end(), (pos + (width - 1))) != m_roads.end() || (pos + (width - 1)) <= 0 || (pos + (width - 1)) >= m_positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos += (width - 1);
						m_roads.push_back(pos);
					}
					break;
				case 1:
					if ((m_roads.size() > (m_positions.size() * 0.35)) || std::find(m_roads.begin(), m_roads.end(), (pos - 1)) != m_roads.end() || (pos - 1) <= 0 || (pos - 1) >= m_positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos -= 1;
						m_roads.push_back(pos);
					}
					break;
				case 2:
					if ((m_roads.size() > (m_positions.size() * 0.35)) || std::find(m_roads.begin(), m_roads.end(), (pos - width)) != m_roads.end() || (pos - width) <= 0 || (pos - width) >= m_positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos -= width;
						m_roads.push_back(pos);
					}
					break;
				case 3:
					if ((m_roads.size() > (m_positions.size() * 0.35)) || std::find(m_roads.begin(), m_roads.end(), (pos - (width - 1))) != m_roads.end() || (pos - (width - 1)) <= 0 || (pos - (width - 1)) >= m_positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos -= (width - 1);
						m_roads.push_back(pos);
					}
					break;
				case 4:
					if ((m_roads.size() > (m_positions.size() * 0.35)) || std::find(m_roads.begin(), m_roads.end(), (pos + 1)) != m_roads.end() || (pos + 1) <= 0 || (pos + 1) >= m_positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos += 1;
						m_roads.push_back(pos);
					}
					break;
				case 5:
					if ((m_roads.size() > (m_positions.size() * 0.35)) || std::find(m_roads.begin(), m_roads.end(), (pos + width)) != m_roads.end() || (pos + width) <= 0 || (pos + width) >= m_positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos += width;
						m_roads.push_back(pos);
					}
					break;
				default:
					ranInto = true;
					midPoint = wolf::RNG::GetRandom(0, m_positions.size());
			}
		}
	}

	//Laying out mountains
	if (!partiallyPregenerated) {
		float minMHeight = m_maxH * 0.75;
		float maxMHeight = m_maxH;
		float percentCoverage = 0.10;

		while (m_mountains.size() < (int)(m_positions.size() * percentCoverage))
		{
			bool found = true;
			int start;

			while (found)
			{
				start = wolf::RNG::GetRandom(0, m_positions.size() - 1);
				if (std::find(m_roads.begin(), m_roads.end(), start) == m_roads.end())
					found = false;
			}

			m_mountains.push_back(start);

			if ((m_mountains.size() < (m_positions.size() * percentCoverage)) && std::find(m_mountains.begin(), m_mountains.end(), (start + (width - 1))) == m_mountains.end() && std::find(m_roads.begin(), m_roads.end(), (start + (width - 1))) == m_roads.end() && (start + (width - 1)) >= 0 && (start + (width - 1)) < m_positions.size())
			{
				m_mountains.push_back(start + (width - 1));
			}

			if ((m_mountains.size() < (m_positions.size() * percentCoverage)) && std::find(m_mountains.begin(), m_mountains.end(), (start - 1)) == m_mountains.end() && std::find(m_roads.begin(), m_roads.end(), (start - 1)) == m_roads.end() && (start - 1) >= 0 && (start - 1) < m_positions.size())
			{
				m_mountains.push_back(start - 1);
			}

			if ((m_mountains.size() < (m_positions.size() * percentCoverage)) && std::find(m_mountains.begin(), m_mountains.end(), (start - width)) == m_mountains.end() && std::find(m_roads.begin(), m_roads.end(), (start - width)) == m_roads.end() && (start - width) >= 0 && (start - width) < m_positions.size())
			{
				m_mountains.push_back(start - width);
			}

			if ((m_mountains.size() < (m_positions.size() * percentCoverage)) && std::find(m_mountains.begin(), m_mountains.end(), (start - (width - 1))) == m_mountains.end() && std::find(m_roads.begin(), m_roads.end(), (start - (width - 1))) == m_roads.end() && (start - (width - 1)) >= 0 && (start - (width - 1)) < m_positions.size())
			{
				m_mountains.push_back(start - (width - 1));
			}

			if ((m_mountains.size() < (m_positions.size() * percentCoverage)) && std::find(m_mountains.begin(), m_mountains.end(), (start + 1)) == m_mountains.end() && std::find(m_roads.begin(), m_roads.end(), (start + 1)) == m_roads.end() && (start + 1) >= 0 && (start + 1) < m_positions.size())
			{
				m_mountains.push_back(start + 1);
			}

			if ((m_mountains.size() < (m_positions.size() * percentCoverage)) && std::find(m_mountains.begin(), m_mountains.end(), (start + width)) == m_mountains.end() && std::find(m_roads.begin(), m_roads.end(), (start + width)) == m_roads.end() && (start + width) >= 0 && (start + width) < m_positions.size())
			{
				m_mountains.push_back(start + width);
			}
		}

		// add a couple mountains to the edges to take out later
		for (int i = 0; i < 20; i++) {
			int pos = GetRandomBorder();
			while (std::find(m_mountains.begin(), m_mountains.end(), pos) != m_mountains.end())
				pos = GetRandomBorder();
			m_mountains.push_back(pos);
		}

		//Laying out Deserts
		int desertStart = wolf::RNG::GetRandom(0, m_positions.size());
		m_desert.push_back(desertStart);
		std::vector<int> nextLayer;
		std::vector<int> currentLayer;
		bool changed = false;
		bool continuing = true;

		while ((m_desert.size() < (int)(m_positions.size() * 0.20)) && continuing)
		{
			if (changed == true)
			{
				changed = false;
			}
			else
			{
				continuing = false;
			}

			for (int i = 0; i < m_desert.size(); i++)
			{
				desertStart = m_desert.at(i);
				if ((m_desert.size() < (m_positions.size() * 0.20)) && std::find(m_desert.begin(), m_desert.end(), (desertStart + (width - 1))) == m_desert.end() && std::find(m_roads.begin(), m_roads.end(), (desertStart + (width - 1))) == m_roads.end() && (desertStart + (width - 1)) >= 0 && (desertStart + (width - 1)) <= m_positions.size())
				{
					m_desert.push_back(desertStart + (width - 1));
					changed = true;
				}

				if ((m_desert.size() < (m_positions.size() * 0.20)) && std::find(m_desert.begin(), m_desert.end(), (desertStart - 1)) == m_desert.end() && std::find(m_roads.begin(), m_roads.end(), (desertStart - 1)) == m_roads.end() && (desertStart - 1) >= 0 && (desertStart - 1) <= m_positions.size())
				{
					m_desert.push_back(desertStart - 1);
					changed = true;
				}

				if ((m_desert.size() < (m_positions.size() * 0.20)) && std::find(m_desert.begin(), m_desert.end(), (desertStart - width)) == m_desert.end() && std::find(m_roads.begin(), m_roads.end(), (desertStart - width)) == m_roads.end() && (desertStart - width) >= 0 && (desertStart - width) <= m_positions.size())
				{
					m_desert.push_back(desertStart - width);
					changed = true;
				}

				if ((m_desert.size() < (m_positions.size() * 0.20)) && std::find(m_desert.begin(), m_desert.end(), (desertStart - (width - 1))) == m_desert.end() && std::find(m_roads.begin(), m_roads.end(), (desertStart - (width - 1))) == m_roads.end() && (desertStart - (width - 1)) >= 0 && (desertStart - (width - 1)) <= m_positions.size())
				{
					m_desert.push_back(desertStart - (width - 1));
					changed = true;
				}

				if ((m_desert.size() < (m_positions.size() * 0.20)) && std::find(m_desert.begin(), m_desert.end(), (desertStart + 1)) == m_desert.end() && std::find(m_roads.begin(), m_roads.end(), (desertStart + 1)) == m_roads.end() && (desertStart + 1) >= 0 && (desertStart + 1) <= m_positions.size())
				{
					m_desert.push_back(desertStart + 1);
					changed = true;
				}

				if ((m_desert.size() < (m_positions.size() * 0.20)) && std::find(m_desert.begin(), m_desert.end(), (desertStart + width)) == m_desert.end() && std::find(m_roads.begin(), m_roads.end(), (desertStart + width)) == m_roads.end() && (desertStart + width) >= 0 && (desertStart + width) <= m_positions.size())
				{
					m_desert.push_back(desertStart + width);
					changed = true;
				}
			}
		}

		for (int i = 0; i < m_mountains.size(); i++)
		{
			if (std::find(m_desert.begin(), m_desert.end(), m_mountains.at(i)) != m_desert.end())
			{
				m_mountains.erase(m_mountains.begin() + i);
			}
		}

		for (int i = 0; i < m_mountains.size(); i++)
		{
			m_heights.at(m_mountains.at(i)) = (isBorder(m_mountains.at(i))) ? 0 : wolf::RNG::GetRandom(minMHeight, maxMHeight);
		}
	}

	// remove any roads that are also occupied by mountains
	for (int i = 0; i < m_roads.size(); i++) {
		if (isMountain(m_roads[i])) {
			m_roads.erase(m_roads.begin() + i--);
		}
	}

	for (int i = 0; i < m_positions.size(); i++)
	{
		if (std::find(m_roads.begin(), m_roads.end(), i) == m_roads.end() && std::find(m_desert.begin(), m_desert.end(), i) == m_desert.end() && std::find(m_mountains.begin(), m_mountains.end(), i) == m_mountains.end())
		{
			m_grass.push_back(i);
		}
	}
}

//Smooth out the heights based on the heights surrounding the tile
void HexGrid::SmoothFullHeights(int width, int numTimes)
{
	for (int i = 0; i < numTimes; i++)
	{
		float totHeight = 0;
		int numCounted = 1;
		for (int i = 0; i < m_heights.size(); i++)
		{
			totHeight += m_heights.at(i);

			if ((i - 1) >= 0)
			{
				totHeight += m_heights.at(i - 1);
				numCounted++;
			}
			if ((i + 1) <= m_heights.size() - 1)
			{
				totHeight += m_heights.at(i + 1);
				numCounted++;
			}
			if ((i + width) <= m_heights.size() - 1)
			{
				totHeight += m_heights.at(i + width);
				numCounted++;
			}
			if ((i + (width - 1)) <= m_heights.size() - 1)
			{
				totHeight += m_heights.at(i + (width - 1));
				numCounted++;
			}
			if ((i - width) >= 0)
			{
				totHeight += m_heights.at(i - width);
				numCounted++;
			}
			if ((i - (width - 1)) >= 0)
			{
				totHeight += m_heights.at(i - (width - 1));
				numCounted++;
			}

			m_heights.at(i) = totHeight / numCounted;

			totHeight = 0;
			numCounted = 1;
		}
	}
}

//Method to Render HexGrid along with Trees and Rocks to Scene
void HexGrid::Render(glm::mat4 p_view, glm::mat4 p_proj, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTexture, float minHeight, float maxHeight)
{
	glm::mat4 projview = p_proj * p_view;
	if (shadowPass)
	{
		if (type == wolf::RenderFilterOpaque) {
			m_pTex->Bind();
			m_dShadowProgram->Bind();

			// Bind Uniforms
			m_dShadowProgram->SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			m_dShadowProgram->SetUniform("model",glm::mat4());

			// Set up source data
			m_pDecl->Bind();

			// Draw!
			glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		}
	}
	else
	{
		if (type == wolf::RenderFilterOpaque) {
			glActiveTexture(GL_TEXTURE0);
			m_pTex->Bind();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMapTexture);
			glActiveTexture(GL_TEXTURE0);

			m_dProgram->Bind();

			glm::mat4 biasMatrix(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			);

			// Bind Uniforms
			m_dProgram->SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			m_dProgram->SetUniform("projection", projview);
			m_dProgram->SetUniform("world", glm::translate(glm::vec3(0.0f)));
			m_dProgram->SetUniform("view", glm::mat4());
			m_dProgram->SetUniform("tex", 0);
			m_dProgram->SetUniform("shadowMap", 1);
			m_dProgram->SetUniform("LightAmbient", m_ambLight);
			m_dProgram->SetUniform("LightDiffuse", m_difLight);
			m_dProgram->SetUniform("LightDir", m_lightDir);
			m_dProgram->SetUniform("minHeight", minHeight);
			m_dProgram->SetUniform("maxHeight", maxHeight);
			glm::mat3 mWorldIT(glm::translate(glm::vec3(0.0f)));
			mWorldIT = glm::inverse(mWorldIT);
			mWorldIT = glm::transpose(mWorldIT);
			m_dProgram->SetUniform("WorldIT", mWorldIT);
			// Set up source data
			m_pDecl->Bind();

			// Draw!
			glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

			for (int i = 0; i < m_selections.size(); i++)
			{
				m_selections.at(i)->Render(projview);
			}
		}
		if (type == wolf::RenderFilterTransparent) {
			m_particleProjMatrix = projview;
			m_particleProjMatrixNoBillboard = glm::mat4(glm::mat3(p_proj * glm::rotate(90.0f, glm::vec3(1, 0, 0))));

			for (int i = 0; i < m_particleEffects.size(); i++)
				m_particleEffects[i]->Render(m_particleProjMatrix, type);
			for (int i = 0; i < m_particleEffectsNoBillboard.size(); i++)
				m_particleEffectsNoBillboard[i]->Render(m_particleProjMatrix, type);
		}
		if (type == wolf::RenderFilterAdditive) {
			for (int i = 0; i < m_particleEffects.size(); i++)
				m_particleEffects[i]->Render(m_particleProjMatrix, type);
			for (int i = 0; i < m_particleEffectsNoBillboard.size(); i++)
				m_particleEffectsNoBillboard[i]->Render(m_particleProjMatrix, type);
		}
		if (type == wolf::RenderFilterDepthless) {
			for (int i = 0; i < m_particleEffects.size(); i++)
				m_particleEffects[i]->Render(m_particleProjMatrix, type);
			for (int i = 0; i < m_particleEffectsNoBillboard.size(); i++)
				m_particleEffectsNoBillboard[i]->Render(m_particleProjMatrix, type);
		}
	}
	for (int i = 0; i < m_trees.size(); i++)
	{
		if (m_trees.at(i)->getTransform()[3][1] - 1.0 < maxHeight)
			m_trees.at(i)->render(projview, glm::mat4(), lightSpaceMatrix, type, shadowPass, depthMapTexture, false);
	}
	for (int i = 0; i < m_rocks.size(); i++)
	{
		m_rocks.at(i)->render(projview, glm::mat4(), lightSpaceMatrix, type, shadowPass, depthMapTexture, true, minHeight, maxHeight);
	}
}

//Method to Return Heights
std::vector<float> HexGrid::GetHeights() const
{
	return m_heights;
}

//Method to Return Position Data
std::vector<glm::vec2> HexGrid::GetPos() const
{
	return m_positions;
}

//Method to get size of map
int HexGrid::GetSize() const
{
	return m_heights.size();
}

//Method to Generate JSON File to use for PathFinding Algorithm
void HexGrid::GenerateHexJSON(int width, int length, float tileWidth)
{
	std::ofstream outputFile;
	outputFile.open("resources/objects/AIPathfindingDataTest.json", std::ofstream::out | std::ofstream::trunc);
	outputFile << "{\"Paths\":{\"Nodes\":[\n";

	std::string currAdd = "";

	for (int i = 0; i < m_positions.size(); i++)
	{
		if (std::find(m_mountains.begin(), m_mountains.end(), i) == m_mountains.end())
		{
			m_workables.push_back(i);
		}
	}

	for (int i = 0; i < m_workables.size(); i++)
	{
		if (i < m_workables.size() - 1)
		{
			outputFile << "{\"ID\":\"" << i << "\",\"X\":\"" << m_positions.at(m_workables.at(i)).x << "\",\"Y\":\"0\",\"Z\":\"" << m_positions.at(m_workables.at(i)).y << "\"},\n";
		}
		else if (i == m_workables.size() - 1)
		{
			outputFile << "{\"ID\":\"" << i << "\",\"X\":\"" << m_positions.at(m_workables.at(i)).x << "\",\"Y\":\"0\",\"Z\":\"" << m_positions.at(m_workables.at(i)).y << "\"}],\n\"Arcs\":[\n";
		}
	}

	for (int i = 0; i < m_workables.size(); i++)
	{
		if (WithinSameLine(m_workables.at(i), m_workables.at(i) + 1, width) && std::find(m_mountains.begin(), m_mountains.end(), m_workables.at(i) + 1) == m_mountains.end() && WithinBounds(m_workables.at(i) + 1))
		{
			//to the right
			int endNode = -1;
			for (int j = 0; j < m_workables.size(); j++)
			{
				if (m_workables.at(j) == m_workables.at(i) + 1)
					endNode = j;
			}
			if (endNode != -1 && endNode < m_workables.size())
			{
				currAdd = currAdd + "{\"start\":\"" + std::to_string(i) + "\",\"end\":\"" + std::to_string(endNode) + "\"},\n";
			}
		}

		if (WithinLineBelow(m_workables.at(i), m_workables.at(i) + width - 1, width) && std::find(m_mountains.begin(), m_mountains.end(), m_workables.at(i) + width - 1) == m_mountains.end() && WithinBounds(m_workables.at(i) + width - 1))
		{
			//bottom left
			int endNode = -1;
			for (int j = 0; j < m_workables.size(); j++)
			{
				if (m_workables.at(j) == m_workables.at(i) + width - 1)
					endNode = j;
			}
			if (endNode != -1 && endNode < m_workables.size())
			{
				currAdd = currAdd + "{\"start\":\"" + std::to_string(i) + "\",\"end\":\"" + std::to_string(endNode) + "\"},\n";
			}
		}

		if (WithinLineBelow(m_workables.at(i), m_workables.at(i) + width, width) && std::find(m_mountains.begin(), m_mountains.end(), m_workables.at(i) + width) == m_mountains.end() && WithinBounds(m_workables.at(i) + width))
		{
			//bottom right
			int endNode = -1;
			for (int j = 0; j < m_workables.size(); j++)
			{
				if (m_workables.at(j) == m_workables.at(i) + width)
					endNode = j;
			}
			if (endNode != -1 && endNode < m_workables.size())
			{
				currAdd = currAdd + "{\"start\":\"" + std::to_string(i) + "\",\"end\":\"" + std::to_string(endNode) + "\"},\n";
			}
		}
	}

	currAdd.pop_back();
	currAdd.pop_back();
	outputFile << currAdd;
	outputFile << "]}}";

}

//Checks whether two tiles are within the same line on a hexgrid
bool HexGrid::WithinSameLine(int tile1, int tile2, int width)
{
	//Within same pair of rows at least
	if ((int)tile1 / (int)(width + width - 1) == (int)tile2 / (int)(width + width - 1))
	{
		if (tile1 % (width + width - 1) < width)
		{
			if ((tile2 % (width + width - 1)) < width)
				return true;
		}
		else
		{
			if ((tile2 % (width + width - 1)) > width)
				return true;
		}
	}

	return false;
}

//Checks whether tiles are on two different lines on a hexgrid
bool HexGrid::WithinLineBelow(int tile1, int tile2, int width)
{
	//capable of being within two rows
	if (tile2 - tile1 < (width + width - 1))
	{
		if (tile1 % (width + width - 1) < width)
		{
			if (tile2 % (width + width - 1) >= width)
				return true;
		}
		else
		{
			if (tile2 % (width + width - 1) < width)
				return true;
		}
	}

	return false;
}

//Checks whether a tile is within the hexgrid
bool HexGrid::WithinBounds(int tile)
{
	if (tile < m_positions.size())
		return true;

	return false;
}

//Method to start targeting
void HexGrid::StartTargeting(int target, int max)
{
	m_targeting = true;
	m_targetingT = target;
	m_targetingMax = max;
}

//Method to stop targeting
void HexGrid::StopTargeting()
{
	m_targeting = false;
	m_targetingT = -1;
}

//Update
void HexGrid::Update(int target, float delta)
{
	//Shows an indicator of which tile is being hovered over
	//When left clicked, will show a path from targeted tile to the one being hovered over then
	if (target != -1) {
		m_abstractTarget = target;

		if (m_lastFrame != target)
		{
			m_changed = true;
		}

		if (m_targeting)
		{
			if (m_changed)
			{
				std::list<glm::vec3> path = m_pathFinder->Instance()->FindPath(glm::vec3(m_positions.at(m_targetingT).x, 0.0f, m_positions.at(m_targetingT).y), glm::vec3(m_positions.at(target).x, 0.0f, m_positions.at(target).y));
				std::vector<glm::vec3> pathway;

				for (auto node : path)
				{
					pathway.push_back(node);
				}

				std::vector<int> tiles;
				for (int i = 0; i < pathway.size() && i < m_targetingMax; i++)
				{
					for (int j = 0; j < m_positions.size(); j++)
					{
						if (cmpf(m_positions.at(j).x, pathway.at(i).x) && cmpf(m_positions.at(j).y, pathway.at(i).z))
						{
							tiles.push_back(j);
						}
					}
				}

				int pSize = 0;
				int endViable = 100;

				for (int i = 1; i < tiles.size(); i++)
				{
					if (pSize <= m_targetingMax)
					{
						if (isDesert(tiles.at(i)))
							pSize += 2;
						else
							pSize += 1;
					}
					if (pSize >= m_targetingMax)
					{
						if (i < endViable)
							endViable = i;
					}
				}

				int tileSize = tiles.size();

				if (tileSize - endViable > 0)
					for (int i = 0; i < tileSize - endViable; i++)
						tiles.pop_back();

				while (tiles.size() > m_selections.size())
				{
					HexSelector* selector = new HexSelector(5.0f);
					m_selections.push_back(selector);
				}

				while (tiles.size() < m_selections.size())
				{
					delete m_selections.at(m_selections.size() - 1);
					m_selections.erase(m_selections.end() - 1);
				}

				for (int i = 0; i < m_selections.size(); i++)
				{
					m_selections.at(i)->Update(tiles.at(i), m_positions.at(tiles.at(i)), m_heights.at(tiles.at(i)));
				}
				m_changed = false;
			}
		}
		else
		{
			for (int i = 0; i < m_selections.size(); i++)
			{
				delete m_selections.at(0);
				m_selections.erase(m_selections.begin());
			}
		}
		m_lastFrame = target;
	}

	for (int i = 0; i < m_particleEffects.size(); i++) {
		m_particleEffects[i]->Update(delta, glm::mat3(m_particleProjMatrix));
	}
	for (int i = 0; i < m_particleEffectsNoBillboard.size(); i++) {
		m_particleEffectsNoBillboard[i]->Update(delta, glm::mat3(m_particleProjMatrixNoBillboard));
	}
}

//Check whether floats are comparable within a value
bool HexGrid::cmpf(float a, float b) const
{
	return (fabs(a - b) < EPSILON_VALUE);
}

//Method to get a pathway given a starting tile and ending tile
std::vector<int> HexGrid::GetPathway(int startTarget, int endTarget)
{
	if (startTarget == -1 || endTarget == -1)
		return std::vector<int>();

	std::list<glm::vec3> path = m_pathFinder->Instance()->FindPath(glm::vec3(m_positions.at(startTarget).x, 0.0f, m_positions.at(startTarget).y), glm::vec3(m_positions.at(endTarget).x, 0.0f, m_positions.at(endTarget).y));
	std::vector<glm::vec3> pathway;

	for (auto node : path)
	{
		pathway.push_back(node);
	}

	std::vector<int> tiles;
	for (int i = 0; i < pathway.size(); i++)
	{
		for (int j = 0; j < m_positions.size(); j++)
		{
			if (cmpf(m_positions.at(j).x, pathway.at(i).x) && cmpf(m_positions.at(j).y, pathway.at(i).z))
			{
				tiles.push_back(j);
			}
		}
	}

	return tiles;
}

//Method to check whether a tile is a mountain
bool HexGrid::isMountain(int pos) const
{
	for (int i = 0; i < m_mountains.size(); i++)
		if (m_mountains[i] == pos)
			return true;

	return false;
}

//Method to check whether a tile is a desert
bool HexGrid::isDesert(int pos) const 
{
	for (int i = 0; i < m_desert.size(); i++)
		if (m_desert[i] == pos)
			return true;

	return false;
}

//Method to block a node based on its position
//Used to block tiles for pathfinding
void HexGrid::BlockNodePositions(glm::vec3 p_nodePos) const
{
	m_pathFinder->Instance()->BlockNode(p_nodePos);
}

//Clears blocked tiles from pathfinding
void HexGrid::ClearBlocks()
{
	m_pathFinder->Instance()->ClearBlockedNodes();
}

//Sets current Light Direction
void HexGrid::SetLightDir(glm::vec3 dir)
{
	for (int i = 0; i < m_trees.size(); i++)
	{
		m_trees.at(i)->setLightDir(dir);
	}
	m_lightDir = dir;
}

//Sets Diffuse Light Color
void HexGrid::SetDiffuse(glm::vec4 dif)
{
	m_difLight = dif;
}

//Sets Ambient Light Color
void HexGrid::SetAmbient(glm::vec4 amb)
{
	m_ambLight = amb;
}

//Method to check whether a tile is on the border of the map or not
bool HexGrid::isBorder(int tile)
{
	if ((tile >= 0 && tile < m_width) || (tile < m_positions.size() && tile > m_positions.size() - m_width))
		return true;

	int sum = 0;
	for (int i = 0; i < m_height; i++) {
		sum += m_width - (i % 2);
		if (tile == sum)
			return true;
	}
		
	sum = 0;
	for (int i = 0; i < m_height; i++) {
		sum += m_width - (i % 2);
		if (tile == sum - 1)
			return true;
	}

	return false;
}