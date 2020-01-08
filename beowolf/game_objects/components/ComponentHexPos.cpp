#include "ComponentHexPos.h"
#include "W_Math.h"

using namespace week2;

ComponentHexPos::ComponentHexPos(){}

ComponentHexPos::~ComponentHexPos(){}

void ComponentHexPos::Update(float p_fDelta)
{
	if (moving)
	{
		timeTaken += p_fDelta;

		if (timeTaken < timeForPath)
		{
			for (int i = 0; i < pathway.size() - 1; i++)
			{
				if (timeTaken >= (i * timePerTile) && timeTaken <= timePerTile + (i * timePerTile))
				{
					std::cout << fmod(timeTaken, timePerTile) / timePerTile;

					currentPos = wolf::Math::lerp(glm::vec3(m_grid->GetPos().at(pathway.at(i)).x, m_grid->GetHeights().at(pathway.at(i)) ,m_grid->GetPos().at(pathway.at(i)).y), glm::vec3(m_grid->GetPos().at(pathway.at(i + 1)).x, m_grid->GetHeights().at(pathway.at(i + 1)), m_grid->GetPos().at(pathway.at(i + 1)).y), (fmod(timeTaken, timePerTile) / timePerTile));
					//std::cout << "Current Pos: " << currentPos.x << ", " << currentPos.y << ", " << currentPos.z << std::endl;
				}
			}
		}
		else
		{
			moving = false;
		}
	}
}

void ComponentHexPos::SetGrid(HexGrid* p_grid)
{
	m_grid = p_grid;
}

void ComponentHexPos::Move(std::vector<int> p_path, float timeToComplete)
{
	moving = true;
	pathway = p_path;
	timeForPath = timeToComplete;
	timeTaken = 0.0f;
	timePerTile = timeForPath / (float)p_path.size();
}

glm::vec3 ComponentHexPos::GetPos()
{
	//return glm::vec3(m_grid->GetPos().at(currentHex).x, m_grid->GetHeights().at(currentHex), m_grid->GetPos().at(currentHex).y);
	return currentPos;
}

bool ComponentHexPos::IsMoving()
{
	return moving;
}