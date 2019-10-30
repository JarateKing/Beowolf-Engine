#include "HexGrid.h"
#include "W_RNG.h"


HexGrid::HexGrid(int width, int length, float tileWidth, float minHeight, float maxHeight, std::string texFile)
{
	GenerateHeights(width, length, minHeight, maxHeight);
	SmoothFullHeights(width, 3);
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

	pTex = wolf::TextureManager::CreateTexture(texFile);
	pTex->SetFilterMode(wolf::Texture::FM_LinearMipmap, wolf::Texture::FM_Linear);
	pTex->SetWrapMode(wolf::Texture::WM_Repeat);
	pTex->Bind();
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

		//test
		//0  vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y + tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
		//1  vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
		//2  vertices.push_back(wolf::Vertex{ (positions.at(i).x - toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
		//3  vertices.push_back(wolf::Vertex{ positions.at(i).x, heights.at(i), (positions.at(i).y - tileWidth / 2), 0, 0, 0, 255, 0.0f, 0.0 });
		//4  vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y - tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });
		//5  vertices.push_back(wolf::Vertex{ (positions.at(i).x + toEdge), heights.at(i), (positions.at(i).y + tileWidth / 4), 0, 0, 0, 255, 0.0f, 0.0 });

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
		std::cout << (int)(positions.size() * 0.35) << std::endl;
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
						std::cout << "Recalculating" << std::endl;
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
		std::cout << roads.size() << std::endl;
		/*for (int i = 0; i < roads.size(); i++)
		{
			std::cout << roads.at(i) << std::endl;
		}*/
	}
	//Laying out mountains


	//Laying out Deserts


	//Setting rest to grass

	
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