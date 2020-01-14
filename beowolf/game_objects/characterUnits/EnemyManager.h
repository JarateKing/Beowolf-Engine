#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <list>
#include "W_Common.h"
#include "CharacterUnits.h"
#include "hexGrid/HexGrid.h"

class EnemyManager
{
public:
	EnemyManager(HexGrid* p_grid);
	~EnemyManager();
	void Update(float deltaT);
	void Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type);
	void Move();

private:
	std::list<CharacterUnits> enemies;
	bool moving = false;
	HexGrid* grid;
	float movementTime = 5.0f;

	std::list<int> FindMaxPathToNearestHero();
};

#endif