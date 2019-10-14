#include "HexGrid.h"

HexGrid::HexGrid(int width, int length, float tileWidth)
{
	GenerateLoc(width, length, tileWidth);
	PrintOutLoc();
}

HexGrid::~HexGrid()
{

}

void HexGrid::GenerateLoc(int width, int length, float tileWidth)
{
	float toEdge = sqrt((pow((tileWidth/2), 2)) - (pow((tileWidth/4), 2)));

	float startX = ((width * (toEdge * 2)) / 2) - (width * (toEdge * 2)) + toEdge;
	//float startY = ((tileWidth* length) / 2) - (tileWidth * length) + (tileWidth / 2);

	float diffX = toEdge * 2;
	float diffY = sqrt(pow(toEdge, 2) + pow((toEdge * 2), 2));

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
	std::cout << "toEdge: " << toEdge << std::endl;
	std::cout << "startX: " << startX << std::endl;
	std::cout << "startY: " << startY << std::endl;
	std::cout << "diffX " << diffX << std::endl;
	std::cout << "diffY " << diffY << std::endl;
	std::cout << "numWide: " << numWide << std::endl;
	std::cout << "numShort: " << numShort << std::endl;

	for (int i = 0; i < numWide; i++)
	{
		for (int j = 0; j < width; j++)
		{
			positions.push_back(glm::vec2((startX + (diffX * j)),(startY + (3 * (tileWidth/2)) * i)));
		}
	}

	for (int i = 0; i < numShort; i++)
	{
		for (int j = 0; j < (width - 1); j++)
		{
			positions.push_back(glm::vec2(((startX + toEdge) + (diffX * j)),((startY + diffY) + (3 * (tileWidth / 2)) * i)));
		}
	}
	
	std::cout << "numpos: " << positions.size() << std::endl;
}

void HexGrid::PrintOutLoc()
{
	for (int i = 0; i < positions.size(); i++)
	{
		std::cout << "{" << positions.at(i).x << ", " << positions.at(i).y << "}" << std::endl;
	}
}