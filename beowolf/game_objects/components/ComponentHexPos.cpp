#include "ComponentHexPos.h"

using namespace week2;

ComponentHexPos::ComponentHexPos(){}

ComponentHexPos::~ComponentHexPos(){}

void ComponentHexPos::Update(float p_fDelta){}

void ComponentHexPos::SetGrid(HexGrid* p_grid)
{
	m_grid = p_grid;
}

void ComponentHexPos::MoveTo(int targetHex)
{
	currentHex = targetHex;
	//if animatable then do animation
}

glm::vec3 ComponentHexPos::GetPos()
{
	return glm::vec3(m_grid->GetPos().at(currentHex).x, m_grid->GetHeights().at(currentHex), m_grid->GetPos().at(currentHex).y);
}