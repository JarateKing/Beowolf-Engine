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

		if (timeTaken <= timeForPath)
		{
			for (int i = 0; i < pathway.size() - 1; i++)
			{
				if (timeTaken >= (i * timePerTile) && timeTaken <= timePerTile + (i * timePerTile))
				{
					glm::vec3 last = currentPos;

					if(pathway.size() > 1)
						currentPos = wolf::Math::lerp(glm::vec3(m_grid->GetPos().at(pathway.at(i)).x, m_grid->GetHeights().at(pathway.at(i)) ,m_grid->GetPos().at(pathway.at(i)).y), glm::vec3(m_grid->GetPos().at(pathway.at(i + 1)).x, m_grid->GetHeights().at(pathway.at(i + 1)), m_grid->GetPos().at(pathway.at(i + 1)).y), (fmod(timeTaken, timePerTile) / timePerTile));
					
					glm::vec3 dif = currentPos - last;
					float dir = 180.0f;
					if (dif.x != 0 || dif.z != 0)
					{
						dif = glm::normalize(dif);
						dir = atan2(dif.z, dif.x) * RAD2DEG;
					}
					dir += 90.0f;
					m_direction = dir;

				}
			}
		}
		else
		{
			if (attacking)
			{
				//std::cout << pathway.at(pathway.size() - 1) << " | " << endAttackTile;

				glm::vec3 last = glm::vec3(m_grid->GetPos().at(pathway.at(pathway.size()-1)).x, m_grid->GetHeights().at(pathway.at(pathway.size()-1)), m_grid->GetPos().at(pathway.at(pathway.size()-1)).y);
				glm::vec3 dif = glm::vec3(m_grid->GetPos().at(endAttackTile).x, m_grid->GetHeights().at(endAttackTile), m_grid->GetPos().at(endAttackTile).y) - last;
				float dir = 180.0f;
				if (dif.x != 0 || dif.z != 0)
				{
					dif = glm::normalize(dif);
					dir = atan2(dif.z, dif.x) * RAD2DEG;
				}
				dir += 90.0f;
				m_direction = dir;
				attacking = false;
			}
			moving = false;
		}
	}
}

void ComponentHexPos::SetGrid(HexGrid* p_grid)
{
	m_grid = p_grid;
}

void ComponentHexPos::Move(std::vector<int> p_path, float timeToComplete, bool p_attacking)
{
	moving = true;
	if (p_attacking)
	{
		attacking = true;
		endAttackTile = p_path.back();
		p_path.pop_back();
	}
	pathway = p_path;
	timeForPath = timeToComplete * p_path.size() + (timeToComplete);
	timeTaken = 0.0f;
	timePerTile = timeToComplete;
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

float ComponentHexPos::GetDirection()
{
	return m_direction;
}

float ComponentHexPos::GetDirection(int startT, int endT)
{
	glm::vec3 last = glm::vec3(m_grid->GetPos().at(startT).x, m_grid->GetHeights().at(startT), m_grid->GetPos().at(startT).y);
	glm::vec3 dif = glm::vec3(m_grid->GetPos().at(endT).x, m_grid->GetHeights().at(endT), m_grid->GetPos().at(endT).y) - last;
	float dir = 180.0f;
	if (dif.x != 0 || dif.z != 0)
	{
		dif = glm::normalize(dif);
		dir = atan2(dif.z, dif.x) * RAD2DEG;
	}
	dir += 90.0f;
	return dir;
}