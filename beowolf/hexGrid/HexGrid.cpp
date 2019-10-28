#include "HexGrid.h"
#include "W_RNG.h"
#include "W_Texture.h"
#include "W_TextureManager.h"

HexGrid::HexGrid(int width, int length, float tileWidth)
{
	GenerateHeights(width, length);
	GenerateLoc(width, length, tileWidth);

	// set up rendering
	g_dProgram = wolf::ProgramManager::CreateProgram("resources/shaders/hex.vsh", "resources/shaders/hex.fsh");
	g_pVB = wolf::BufferManager::CreateVertexBuffer(p_verts, sizeof(wolf::Vertex) * vertices.size());

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	g_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	g_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
	g_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	g_pDecl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();

	wolf::Texture* pTex = wolf::TextureManager::CreateTexture("resources/textures/tiles/Tile_Texs_1.tga");
	pTex->SetFilterMode(wolf::Texture::FM_LinearMipmap, wolf::Texture::FM_Linear);
	pTex->SetWrapMode(wolf::Texture::WM_Repeat);
	pTex->Bind();
}

HexGrid::~HexGrid()
{

}

void HexGrid::GenerateHeights(int width, int length)
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
		heights.push_back(wolf::RNG::GetRandom(0.0f, 5.0f));
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

	if (length % 2 == 0)
	{
		numWide = length / 2;
	}
	else
	{
		numWide = (length / 2) + 1;
	}

	numShort = length - numWide;
	float startY = ((((tileWidth * numWide) + ((tileWidth / 2) * numShort)) / 2) - ((tileWidth * numWide) + ((tileWidth / 2) * numShort))) + (tileWidth / 2);

	for (int i = 0; i < numWide; i++)
	{
		for (int j = 0; j < width; j++)
		{
			positions.push_back(glm::vec2((startX + (diffX * j)), (startY + (3 * (tileWidth/2)) * i)));
		}

		for (int j = 0; j < (width - 1); j++)
		{
			positions.push_back(glm::vec2(((startX + toEdge) + (diffX * j)), ((startY + diffY) + (3 * (tileWidth / 2)) * i)));
		}
	}

	GenerateVerts(tileWidth, toEdge);
}

void HexGrid::PrintOutLoc()
{
	for (int i = 0; i < positions.size(); i++)
	{
		std::cout << "{" << positions.at(i).x << ", " << positions.at(i).y << "}" << std::endl;
	}
}

void HexGrid::GenerateVerts(float tileWidth, float toEdge)
{
	for (int i = 0; i < positions.size(); i++)
	{
		//Top Verts
		verts[0] = { positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 255, 255, 255, 255, 0.25f, 0.0f };
		verts[1] = { (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 255, 255, 255, 255, 0.0f, 0.125f };
		verts[2] = { (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 255, 255, 255, 255, 0.0f, 0.375f };
		verts[3] = { positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 255, 255, 255, 255, 0.25f, 0.5f };
		verts[4] = { (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 255, 255, 255, 255, 0.5f, 0.375f };
		verts[5] = { (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 255, 255, 255, 255, 0.5f, 0.125f };

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

		//Front left Vertices
		vertices.push_back(verts[0]);
		vertices.push_back(verts[1]);
		vertices.push_back(verts[7]);
		vertices.push_back(verts[0]);
		vertices.push_back(verts[7]);
		vertices.push_back(verts[6]);

		//Left Vertices
		vertices.push_back(verts[1]);
		vertices.push_back(verts[8]);
		vertices.push_back(verts[2]);
		vertices.push_back(verts[1]);
		vertices.push_back(verts[8]);
		vertices.push_back(verts[7]);

		//Back left Vertices
		vertices.push_back(verts[2]);
		vertices.push_back(verts[9]);
		vertices.push_back(verts[3]);
		vertices.push_back(verts[2]);
		vertices.push_back(verts[9]);
		vertices.push_back(verts[8]);

		//Back right Vertices
		vertices.push_back(verts[3]);
		vertices.push_back(verts[10]);
		vertices.push_back(verts[4]);
		vertices.push_back(verts[3]);
		vertices.push_back(verts[10]);
		vertices.push_back(verts[9]);

		//Right Vertices
		vertices.push_back(verts[4]);
		vertices.push_back(verts[11]);
		vertices.push_back(verts[5]);
		vertices.push_back(verts[4]);
		vertices.push_back(verts[11]);
		vertices.push_back(verts[10]);

		//Front right Vertices
		vertices.push_back(verts[5]);
		vertices.push_back(verts[6]);
		vertices.push_back(verts[0]);
		vertices.push_back(verts[5]);
		vertices.push_back(verts[6]);
		vertices.push_back(verts[11]);
	}

	std::cout << vertices.size() << std::endl;

	for (int i = 0; i < vertices.size(); i++)
	{
		p_verts[i] = vertices.at(i);
	}
}

void HexGrid::Render(glm::mat4 projview)
{
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
}