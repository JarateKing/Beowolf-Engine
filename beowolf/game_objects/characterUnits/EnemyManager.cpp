#include "EnemyManager.h"
#include "W_Input.h"

EnemyManager::EnemyManager(HexGrid* p_grid)
{
	CharacterUnits lich("lich/FreeLich.bmw", "animatable", 1, "Lich", p_grid);
	enemies.push_back(lich);
	grid = p_grid;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Update(float p_deltaT)
{
	std::list<CharacterUnits>::iterator it;
	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		it->Update(p_deltaT);
	}

	if (moving)
	{

	}
}

void EnemyManager::Render(glm::mat4 p_view, glm::mat4 p_proj, bool p_renderAlphas)
{
	std::list<CharacterUnits>::iterator it;
	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		it->Render(p_view, p_proj, p_renderAlphas);
	}
}

void EnemyManager::Move()
{
	moving = true;
}

std::list<int> EnemyManager::FindMaxPathToNearestHero()
