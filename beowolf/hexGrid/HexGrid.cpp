#include "HexGrid.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"
#include "W_Input.h"
#include <fstream>
#include <string>
#define EPSILON_VALUE 0.01


HexGrid::HexGrid(int width, int length, float tileWidth, float minHeight, float maxHeight, std::string texFile)
{
	pathFinder->CreateInstance();
	minH = minHeight;
	maxH = maxHeight;

	GenerateHeights(width, length, minHeight, maxHeight);
	SmoothFullHeights(width, 3);
	GenerateLoc(width, length, tileWidth);
	GenerateHexJSON(width, length, tileWidth);
	pathFinder->Instance()->Load("resources/objects/AIPathfindingDataTest.json");

	int treeNum = wolf::RNG::GetRandom(0, (positions.size() - mountains.size() - desert.size() - roads.size()) / 2);
	std::vector<int> treePos;
	for (int i = 0; i < treeNum; i++)
	{
		bool passed = false;
		while (!passed)
		{
			int next = wolf::RNG::GetRandom(0, positions.size());
			if (std::find(treePos.begin(), treePos.end(), next) == treePos.end() && std::find(grass.begin(), grass.end(), next) != grass.end())
			{
				treePos.push_back(next);
				passed = true;
			}
		}
	}

	int rnd;
	std::string model;
	for (int i = 0; i < treeNum; i++)
	{
		auto shaders = wolf::ResourceLoader::Instance().getShaders("unlit_texture");
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

		test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(model), shaders.first, shaders.second);
		float setScale = wolf::RNG::GetRandom(0.01f, 0.02f);
		test->setTransform(glm::translate(glm::vec3(positions.at(treePos.at(i)).x, heights.at(treePos.at(i)), positions.at(treePos.at(i)).y)) * glm::rotate(180.0f, glm::vec3(0, wolf::RNG::GetRandom(0.0f, 5.0f), 0)) * glm::scale(glm::vec3(setScale, setScale, setScale)));
		trees.push_back(test);
	}

	// set up rendering
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("hex"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(p_verts, sizeof(wolf::Vertex) * vertices.size());

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();

	pTex = wolf::TextureManager::CreateTexture(texFile);
	pTex->SetFilterMode(wolf::Texture::FM_LinearMipmap, wolf::Texture::FM_Linear);
	pTex->SetWrapMode(wolf::Texture::WM_Repeat);
}

HexGrid::~HexGrid()
{

}

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
		heights.push_back(wolf::RNG::GetRandom(minHeight, maxHeight));
	}
}

void HexGrid::GenerateLoc(int width, int length, float tileWidth)
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
				positions.push_back(glm::vec2((startX + (diffX * j)), (startY + (diffY * i))));
			}			
			k = 1;
		}
		else
		{
			for (int j = 0; j < (width - 1); j++)
			{
				positions.push_back(glm::vec2(((startX + toEdge) + (diffX * j)), (startY + (diffY * i))));
			}
			k = 0;
		}
	}
	GroupTextures(width);
	GenerateVerts(tileWidth, toEdge);
}

void HexGrid::PrintOutLoc()
{
	/*for (int i = 0; i < positions.size(); i++)
	{
		std::cout << "{" << positions.at(i).x << ", " << positions.at(i).y << "}" << std::endl;
	}*/

	for (int i = 0; i < heights.size(); i++)
	{
		std::cout << heights.at(i) << std::endl;
	}
}

void HexGrid::GenerateVerts(float tileWidth, float toEdge)
{
	for (int i = 0; i < positions.size(); i++)
	{
		int ranTexX = wolf::RNG::GetRandom(0, 1);
		int ranTexY = wolf::RNG::GetRandom(0, 1);
		
		glm::vec2 p0Tex(0.25f + (0.5f * ranTexX) , 0.0f + (0.5f * ranTexY));
		glm::vec2 p1Tex(0.0f + (0.5f * ranTexX),0.125f + (0.5f * ranTexY));
		glm::vec2 p2Tex(0.0f + (0.5f * ranTexX),0.375f + (0.5f * ranTexY));
		glm::vec2 p3Tex(0.25f + (0.5f * ranTexX),0.5f + (0.5f * ranTexY));
		glm::vec2 p4Tex(0.5f + (0.5f * ranTexX),0.375f + (0.5f * ranTexY));
		glm::vec2 p5Tex(0.5f + (0.5f * ranTexX),0.125f + (0.5f * ranTexY));

		if (std::find(roads.begin(), roads.end(), i) != roads.end())
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
		else if (std::find(mountains.begin(), mountains.end(), i) != mountains.end())
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
		else if (std::find(desert.begin(), desert.end(), i) != desert.end())
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
		verts[0] = { positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 255, 255, 255, 255, p0Tex.x, p0Tex.y };
		verts[1] = { (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 255, 255, 255, 255, p1Tex.x, p1Tex.y };
		verts[2] = { (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 255, 255, 255, 255, p2Tex.x, p2Tex.y };
		verts[3] = { positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 255, 255, 255, 255, p3Tex.x, p3Tex.y };
		verts[4] = { (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 255, 255, 255, 255, p4Tex.x, p4Tex.y };
		verts[5] = { (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 255, 255, 255, 255, p5Tex.x, p5Tex.y };

		//Bottom Verts
		verts[6] = { positions.at(i).x, 0.0f, (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f };
		verts[7] = { (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };
		verts[8] = { (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };
		verts[9] = { positions.at(i).x, 0.0f, (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0f };
		verts[10] = { (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };
		verts[11] = { (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0f };

		//Top Vertices
		vertices.push_back(verts[3]);
		vertices.push_back(verts[4]);
		vertices.push_back(verts[5]);
		vertices.push_back(verts[3]);
		vertices.push_back(verts[5]);
		vertices.push_back(verts[0]);
		vertices.push_back(verts[3]);
		vertices.push_back(verts[1]);
		vertices.push_back(verts[0]);
		vertices.push_back(verts[3]);
		vertices.push_back(verts[2]);
		vertices.push_back(verts[1]);

		if (std::find(mountains.begin(), mountains.end(), i) != mountains.end())
		{
			//front left vertices
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, 0.0f, (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.6f, 0.0f });

			//Left Vertices
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f });

			//Back left Vertices
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, 0.0f, (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 1.0f, 0.0f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, 0.0f, (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 1.0f, 0.0f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f });

			//Back right Vertices
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, 0.0f, (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 1.0f, 0.0f });

			//Right Vertices
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.6f, 0.0f });

			//Front right Vertices
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, 0.0f, (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.6f, 0.0f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 200, 200, 200, 255, 0.6f, 0.45f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 200, 200, 200, 255, 1.0f, 0.45f });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, 0.0f, (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.6f, 0.0f });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), 0.0f, (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 1.0f, 0.0f });
		}
		else
		{
			//Front left Vertices
			//vertices.push_back(verts[0]);
			//vertices.push_back(verts[1]);
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[7]);
			//vertices.push_back(verts[0]);
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[7]);
			vertices.push_back(verts[6]);

			//Left Vertices
			//vertices.push_back(verts[1]);
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[8]);
			//vertices.push_back(verts[2]);
			//vertices.push_back(verts[1]);
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[8]);
			vertices.push_back(verts[7]);

			//Back left Vertices
			//vertices.push_back(verts[2]);
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[9]);
			//vertices.push_back(verts[3]);
			//vertices.push_back(verts[2]);
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[9]);
			vertices.push_back(verts[8]);

			//Back right Vertices
			//vertices.push_back(verts[3]);
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[10]);
			//vertices.push_back(verts[4]);
			//vertices.push_back(verts[3]);
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[10]);
			vertices.push_back(verts[9]);

			//Right Vertices
			//vertices.push_back(verts[4]);
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[11]);
			//vertices.push_back(verts[5]);
			//vertices.push_back(verts[4]);
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[11]);
			vertices.push_back(verts[10]);

			//Front right Vertices
			//vertices.push_back(verts[5]);
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[6]);
			//vertices.push_back(verts[0]);
			//vertices.push_back(verts[5]);
			vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
			vertices.push_back(verts[6]);
			vertices.push_back(verts[11]);
		}
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		p_verts[i] = vertices.at(i);
	}
}

void HexGrid::GroupTextures(int width)
{
	bool ranInto = false;
	int midPoint = (int)(positions.size()/2);
	roads.push_back(midPoint);

	int path;
	int lastTaken = -1;

	//Creating road layout
	while ((roads.size() < (int)(positions.size() * 0.35)))
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

			if ((std::find(roads.begin(), roads.end(), (pos + (width - 1))) != roads.end() || (pos + (width - 1)) >= positions.size())  &&
				(std::find(roads.begin(), roads.end(), (pos - 1)) != roads.end() || (pos - 1) <= 0) &&
				(std::find(roads.begin(), roads.end(), (pos - width)) != roads.end() || (pos - width) <= 0) &&
				(std::find(roads.begin(), roads.end(), (pos - (width - 1))) != roads.end() || (pos - (width - 1)) <= 0) &&
				(std::find(roads.begin(), roads.end(), (pos + 1)) != roads.end() || (pos + 1) >= positions.size()) &&
				(std::find(roads.begin(), roads.end(), (pos + width)) != roads.end() || (pos + width) >= positions.size()))
			{
				midPoint = wolf::RNG::GetRandom(0, positions.size());
			}

			switch (path)
			{
				case 0:
					if ((roads.size() > (positions.size() * 0.35)) || std::find(roads.begin(), roads.end(), (pos + (width - 1))) != roads.end() || (pos + (width - 1)) <= 0 || (pos + (width - 1)) >= positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos += (width - 1);
						roads.push_back(pos);
					}
					break;
				case 1:
					if ((roads.size() > (positions.size() * 0.35)) || std::find(roads.begin(), roads.end(), (pos - 1)) != roads.end() || (pos - 1) <= 0 || (pos - 1) >= positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos -= 1;
						roads.push_back(pos);
					}
					break;
				case 2:
					if ((roads.size() > (positions.size() * 0.35)) || std::find(roads.begin(), roads.end(), (pos - width)) != roads.end() || (pos - width) <= 0 || (pos - width) >= positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos -= width;
						roads.push_back(pos);
					}
					break;
				case 3:
					if ((roads.size() > (positions.size() * 0.35)) || std::find(roads.begin(), roads.end(), (pos - (width - 1))) != roads.end() || (pos - (width - 1)) <= 0 || (pos - (width - 1)) >= positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos -= (width - 1);
						roads.push_back(pos);
					}
					break;
				case 4:
					if ((roads.size() > (positions.size() * 0.35)) || std::find(roads.begin(), roads.end(), (pos + 1)) != roads.end() || (pos + 1) <= 0 || (pos + 1) >= positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos += 1;
						roads.push_back(pos);
					}
					break;
				case 5:
					if ((roads.size() > (positions.size() * 0.35)) || std::find(roads.begin(), roads.end(), (pos + width)) != roads.end() || (pos + width) <= 0 || (pos + width) >= positions.size())
					{
						ranInto = true;
					}
					else
					{
						pos += width;
						roads.push_back(pos);
					}
					break;
				default:
					ranInto = true;
					midPoint = wolf::RNG::GetRandom(0, positions.size());
			}
		}
	}

	//Laying out mountains

	float minMHeight = maxH * 0.75;
	float maxMHeight = maxH;

	while ((mountains.size() < (int)(positions.size() * 0.20)))
	{
		bool found = true;
		int start;

		while (found)
		{
			start = wolf::RNG::GetRandom(0, positions.size() - 1);
			if (std::find(roads.begin(), roads.end(), start) == roads.end())
				found = false;
		}

		mountains.push_back(start);

		if ((mountains.size() < (positions.size() * 0.20)) && std::find(mountains.begin(), mountains.end(), (start + (width - 1))) == mountains.end() && std::find(roads.begin(), roads.end(), (start + (width - 1))) == roads.end() && (start + (width - 1)) >= 0 && (start + (width - 1)) < positions.size())
		{
			mountains.push_back(start + (width - 1));
		}

		if ((mountains.size() < (positions.size() * 0.20)) && std::find(mountains.begin(), mountains.end(), (start - 1)) == mountains.end() && std::find(roads.begin(), roads.end(), (start  - 1)) == roads.end() && (start - 1) >= 0 && (start - 1) < positions.size())
		{
			mountains.push_back(start - 1);
		}

		if ((mountains.size() < (positions.size() * 0.20)) && std::find(mountains.begin(), mountains.end(), (start - width)) == mountains.end() && std::find(roads.begin(), roads.end(), (start - width)) == roads.end() && (start - width) >= 0 && (start - width) < positions.size())
		{
			mountains.push_back(start - width);
		}

		if ((mountains.size() < (positions.size() * 0.20)) && std::find(mountains.begin(), mountains.end(), (start - (width - 1))) == mountains.end() && std::find(roads.begin(), roads.end(), (start - (width - 1))) == roads.end() && (start - (width - 1)) >= 0 && (start - (width - 1)) < positions.size())
		{
			mountains.push_back(start - (width - 1));
		}

		if ((mountains.size() < (positions.size() * 0.20)) && std::find(mountains.begin(), mountains.end(), (start + 1)) == mountains.end() && std::find(roads.begin(), roads.end(), (start + 1)) == roads.end() && (start + 1) >= 0 && (start + 1) < positions.size())
		{
			mountains.push_back(start + 1);
		}

		if ((mountains.size() < (positions.size() * 0.20)) && std::find(mountains.begin(), mountains.end(), (start + width)) == mountains.end() && std::find(roads.begin(), roads.end(), (start + width)) == roads.end() && (start + width) >= 0 && (start + width) < positions.size())
		{
			mountains.push_back(start + width);
		}
	}

	//Laying out Deserts

	int desertStart = wolf::RNG::GetRandom(0, positions.size());
	desert.push_back(desertStart);
	std::vector<int> nextLayer;
	std::vector<int> currentLayer;
	bool changed = false;
	bool continuing = true;

	while ((desert.size() < (int)(positions.size() * 0.20)) && continuing)
	{
		if (changed == true)
		{
			changed = false;
		}
		else
		{
			continuing = false;
		}

		for (int i = 0; i < desert.size(); i++)
		{
			desertStart = desert.at(i);
			if ((desert.size() < (positions.size() * 0.20)) && std::find(desert.begin(), desert.end(), (desertStart + (width - 1))) == desert.end() && std::find(roads.begin(), roads.end(), (desertStart + (width - 1))) == roads.end() && (desertStart + (width - 1)) >= 0 && (desertStart + (width - 1)) <= positions.size())
			{
				desert.push_back(desertStart + (width - 1));
				changed = true;
			}

			if ((desert.size() < (positions.size() * 0.20)) && std::find(desert.begin(), desert.end(), (desertStart - 1)) == desert.end() && std::find(roads.begin(), roads.end(), (desertStart - 1)) == roads.end() && (desertStart - 1) >= 0 && (desertStart - 1) <= positions.size())
			{
				desert.push_back(desertStart - 1);
				changed = true;
			}

			if ((desert.size() < (positions.size() * 0.20)) && std::find(desert.begin(), desert.end(), (desertStart - width)) == desert.end() && std::find(roads.begin(), roads.end(), (desertStart - width)) == roads.end() && (desertStart - width) >= 0 && (desertStart - width) <= positions.size())
			{
				desert.push_back(desertStart - width);
				changed = true;
			}

			if ((desert.size() < (positions.size() * 0.20)) && std::find(desert.begin(), desert.end(), (desertStart - (width - 1))) == desert.end() && std::find(roads.begin(), roads.end(), (desertStart - (width - 1))) == roads.end() && (desertStart - (width - 1)) >= 0 && (desertStart - (width - 1)) <= positions.size())
			{
				desert.push_back(desertStart - (width - 1));
				changed = true;
			}

			if ((desert.size() < (positions.size() * 0.20)) && std::find(desert.begin(), desert.end(), (desertStart + 1)) == desert.end() && std::find(roads.begin(), roads.end(), (desertStart + 1)) == roads.end() && (desertStart + 1) >= 0 && (desertStart + 1) <= positions.size())
			{
				desert.push_back(desertStart + 1);
				changed = true;
			}

			if ((desert.size() < (positions.size() * 0.20)) && std::find(desert.begin(), desert.end(), (desertStart + width)) == desert.end() && std::find(roads.begin(), roads.end(), (desertStart + width)) == roads.end() && (desertStart + width) >= 0 && (desertStart + width) <= positions.size())
			{
				desert.push_back(desertStart + width);
				changed = true;
			}
		}
	}

	for (int i = 0; i < mountains.size(); i++)
	{
		if (std::find(desert.begin(), desert.end(), mountains.at(i)) != desert.end())
		{
			mountains.erase(mountains.begin() + i);
		}
	}

	//NEED TO FIX ISSUE HERE
	for (int i = 0; i < mountains.size(); i++)
	{
		heights.at(mountains.at(i)) = wolf::RNG::GetRandom(minMHeight, maxMHeight);
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (std::find(roads.begin(), roads.end(), i) == roads.end() && std::find(desert.begin(), desert.end(), i) == desert.end() && std::find(mountains.begin(), mountains.end(), i) == mountains.end())
		{
			grass.push_back(i);
		}
	}
}

void HexGrid::SmoothFullHeights(int width, int numTimes)
{
	for (int i = 0; i < numTimes; i++)
	{
		float totHeight = 0;
		int numCounted = 1;
		for (int i = 0; i < heights.size(); i++)
		{
			totHeight += heights.at(i);

			if ((i - 1) >= 0)
			{
				totHeight += heights.at(i - 1);
				numCounted++;
			}
			if ((i + 1) <= heights.size() - 1)
			{
				totHeight += heights.at(i + 1);
				numCounted++;
			}
			if ((i + width) <= heights.size() - 1)
			{
				totHeight += heights.at(i + width);
				numCounted++;
			}
			if ((i + (width - 1)) <= heights.size() - 1)
			{
				totHeight += heights.at(i + (width - 1));
				numCounted++;
			}
			if ((i - width) >= 0)
			{
				totHeight += heights.at(i - width);
				numCounted++;
			}
			if ((i - (width - 1)) >= 0)
			{
				totHeight += heights.at(i - (width - 1));
				numCounted++;
			}

			heights.at(i) = totHeight / numCounted;

			totHeight = 0;
			numCounted = 1;
		}
	}
}

void HexGrid::Render(glm::mat4 projview)
{
	pTex->Bind();
	g_dProgram->Bind();

	// Bind Uniforms
	g_dProgram->SetUniform("projection", projview);
	g_dProgram->SetUniform("world", glm::mat4());
	g_dProgram->SetUniform("view", glm::mat4());
	g_dProgram->SetUniform("tex", 0);

	// Set up source data
	g_pDecl->Bind();

	// Draw!
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	for (int i = 0; i < selections.size(); i++)
	{
		selections.at(i)->Render(projview);
	}
	for (int i = 0; i < trees.size(); i++)
	{
		glDepthMask(true);
		glDisable(GL_BLEND);
		trees.at(i)->render(projview, glm::mat4(), false);
	}

	for (int i = 0; i < trees.size(); i++)
	{
		glDepthMask(false);
		glEnable(GL_BLEND);
		trees.at(i)->render(projview, glm::mat4(), true);
	}
}

std::vector<float> HexGrid::GetHeights()
{
	return heights;
}

std::vector<glm::vec2> HexGrid::GetPos()
{
	return positions;
}

void HexGrid::GenerateHexJSON(int width, int length, float tileWidth)
{
	std::ofstream outputFile;
	outputFile.open("resources/objects/AIPathfindingDataTest.json", std::ofstream::out | std::ofstream::trunc);
	outputFile << "{\"Paths\":{\"Nodes\":[\n";

	std::string currAdd = "";

	for (int i = 0; i < positions.size(); i++)
	{
		if (std::find(mountains.begin(), mountains.end(), i) == mountains.end())
		{
			workables.push_back(i);
		}
	}

	for (int i = 0; i < workables.size(); i++)
	{
		if (i < workables.size() - 1)
		{
			outputFile << "{\"ID\":\"" << i << "\",\"X\":\"" << positions.at(workables.at(i)).x << "\",\"Y\":\"0\",\"Z\":\"" << positions.at(workables.at(i)).y << "\"},\n";
		}
		else if (i == workables.size() - 1)
		{
			outputFile << "{\"ID\":\"" << i << "\",\"X\":\"" << positions.at(workables.at(i)).x << "\",\"Y\":\"0\",\"Z\":\"" << positions.at(workables.at(i)).y << "\"}],\n\"Arcs\":[\n";
		}
	}

	for (int i = 0; i < workables.size(); i++)
	{
		if (WithinSameLine(workables.at(i), workables.at(i) + 1, width) && std::find(mountains.begin(), mountains.end(), workables.at(i) + 1) == mountains.end() && WithinBounds(workables.at(i) + 1))
		{
			//to the right
			int endNode = -1;
			for (int j = 0; j < workables.size(); j++)
			{
				if (workables.at(j) == workables.at(i) + 1)
					endNode = j;
			}
			if (endNode != -1 && endNode < workables.size())
			{
				currAdd = currAdd + "{\"start\":\"" + std::to_string(i) + "\",\"end\":\"" + std::to_string(endNode) + "\"},\n";
			}
		}

		if (WithinLineBelow(workables.at(i), workables.at(i) + width - 1, width) && std::find(mountains.begin(), mountains.end(), workables.at(i) + width - 1) == mountains.end() && WithinBounds(workables.at(i) + width - 1))
		{
			//bottom left
			int endNode = -1;
			for (int j = 0; j < workables.size(); j++)
			{
				if (workables.at(j) == workables.at(i) + width - 1)
					endNode = j;
			}
			if (endNode != -1 && endNode < workables.size())
			{
				currAdd = currAdd + "{\"start\":\"" + std::to_string(i) + "\",\"end\":\"" + std::to_string(endNode) + "\"},\n";
			}
		}

		if (WithinLineBelow(workables.at(i), workables.at(i) + width, width) && std::find(mountains.begin(), mountains.end(), workables.at(i) + width) == mountains.end() && WithinBounds(workables.at(i) + width))
		{
			//bottom right
			int endNode = -1;
			for (int j = 0; j < workables.size(); j++)
			{
				if (workables.at(j) == workables.at(i) + width)
					endNode = j;
			}
			if (endNode != -1 && endNode < workables.size())
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

bool HexGrid::WithinBounds(int tile)
{
	if (tile < positions.size())
		return true;

	return false;
}

void HexGrid::Update(int target, float delta)
{
	abstractTarget = target;

	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == false && timeBetween >= 1.0f)
	{
		//std::cout << "Updating" << std::endl;
		targeting = true;
		targetingT = target;
		timeBetween = 0.0f;
	}

	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == true && timeBetween >= 1.0f)
	{
		//std::cout << "Updating" << std::endl;
		targeting = false;
		targetingT = -1;
		timeBetween = 0.0f;
	}

	if (targeting)
	{
		std::list<glm::vec3> path = pathFinder->Instance()->FindPath(glm::vec3(positions.at(targetingT).x, 0.0f, positions.at(targetingT).y), glm::vec3(positions.at(target).x, 0.0f, positions.at(target).y));
		std::vector<glm::vec3> pathway;

		for (auto node : path)
		{
			pathway.push_back(node);
		}

		std::vector<int> tiles;
		for (int i = 0; i < pathway.size(); i++)
		{
			for (int j = 0; j < positions.size(); j++)
			{
				if (cmpf(positions.at(j).x, pathway.at(i).x) && cmpf(positions.at(j).y,pathway.at(i).z))
				{
					tiles.push_back(j);
				}
			}
		}

		timeBetween += delta;
		
		while (tiles.size() > selections.size())
		{
			HexSelector* selector = new HexSelector(5.0f);
			selections.push_back(selector);
		}

		while (tiles.size() < selections.size())
		{
			delete selections.at(selections.size() - 1);
			selections.erase(selections.end() - 1);
		}

		for (int i = 0; i < selections.size(); i++)
		{
			selections.at(i)->Update(tiles.at(i), positions.at(tiles.at(i)), heights.at(tiles.at(i)));
		}
	}
	else
	{
		timeBetween += delta;

		for (int i = 0; i < selections.size(); i++)
		{
			delete selections.at(0);
			selections.erase(selections.begin());
		}
	}
}

bool HexGrid::cmpf(float a, float b)
{
	return (fabs(a - b) < EPSILON_VALUE);
}