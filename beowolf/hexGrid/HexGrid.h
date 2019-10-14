#ifndef HEXGRID_H
#define HEXGRID_H

#include <iostream>
#include <vector>
#include "W_Common.h"

class HexGrid
{
public:
	HexGrid(int width, int length, float tileWidth);
	~HexGrid();
	void PrintOutLoc();
private:
	void GenerateLoc(int width, int length, float tileWidth);
	std::vector<glm::vec2> positions;
	std::vector<int> heights;
};

#endif