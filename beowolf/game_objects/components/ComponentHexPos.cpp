#include "ComponentHexPos.h"
#include "W_Math.h"

using namespace week2;

ComponentHexPos::ComponentHexPos(){}

ComponentHexPos::~ComponentHexPos(){}

void ComponentHexPos::Update(float p_fDelta)
{
	if (m_moving)
	{
		m_timeTaken += p_fDelta;

		//If movement time remaining continue
		if (m_timeTaken <= m_timeForPath)
		{
			for (int i = 0; i < m_pathway.size() - 1; i++)
			{
				//While moving make sure character is pointed towards the next tile and moving evenly
				if (m_timeTaken >= (i * m_timePerTile) && m_timeTaken <= m_timePerTile + (i * m_timePerTile))
				{
					glm::vec3 last = m_currentPos;

					if(m_pathway.size() > 1)
						m_currentPos = wolf::Math::lerp(glm::vec3(m_grid->GetPos().at(m_pathway.at(i)).x, m_grid->GetHeights().at(m_pathway.at(i)) ,m_grid->GetPos().at(m_pathway.at(i)).y), glm::vec3(m_grid->GetPos().at(m_pathway.at(i + 1)).x, m_grid->GetHeights().at(m_pathway.at(i + 1)), m_grid->GetPos().at(m_pathway.at(i + 1)).y), (fmod(m_timeTaken, m_timePerTile) / m_timePerTile));
					
					glm::vec3 dif = m_currentPos - last;
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
			//if character is attacking aim towards where attacking
			if (m_attacking)
			{
				glm::vec3 last = glm::vec3(m_grid->GetPos().at(m_pathway.at(m_pathway.size()-1)).x, m_grid->GetHeights().at(m_pathway.at(m_pathway.size()-1)), m_grid->GetPos().at(m_pathway.at(m_pathway.size()-1)).y);
				glm::vec3 dif = glm::vec3(m_grid->GetPos().at(m_endAttackTile).x, m_grid->GetHeights().at(m_endAttackTile), m_grid->GetPos().at(m_endAttackTile).y) - last;
				m_currentPos = last;
				float dir = 180.0f;
				if (dif.x != 0 || dif.z != 0)
				{
					dif = glm::normalize(dif);
					dir = atan2(dif.z, dif.x) * RAD2DEG;
				}
				dir += 90.0f;
				m_direction = dir;
				m_attacking = false;
			}
			m_moving = false;
			m_justEndMove = true;
		}
	}
}

//Method to Set the Grid Used
void ComponentHexPos::SetGrid(HexGrid* p_grid)
{
	m_grid = p_grid;
}

//Method to Set current Position
void ComponentHexPos::SetPos(glm::vec3 pos)
{
	m_currentPos = pos;
}

//Method to move a character based on a path and time to complete
void ComponentHexPos::Move(std::vector<int> p_path, float timeToComplete, bool p_attacking)
{
	m_moving = true;
	if (p_attacking)
	{
		m_attacking = true;
		m_endAttackTile = p_path.back();
		p_path.pop_back();
	}
	m_pathway = p_path;
	m_timeForPath = timeToComplete * p_path.size() + (timeToComplete);
	m_timeTaken = 0.0f;
	m_timePerTile = timeToComplete;
}

//Method to return Position
glm::vec3 ComponentHexPos::GetPos()
{
	return m_currentPos;
}

//Method to check if moving
bool ComponentHexPos::IsMoving()
{
	return m_moving;
}

//Method to return current direction
float ComponentHexPos::GetDirection()
{
	return m_direction;
}

//Method to return a direction based on a start and end tile
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