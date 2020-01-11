#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <list>
#include "W_Common.h"
#include "CharacterUnits.h"
#include "hexGrid/HexGrid.h"

class CharacterManager
{
	public:
		CharacterManager(HexGrid* p_grid);
		~CharacterManager();
		void Update(int target, float deltaT);
		void Render(glm::mat4 p_view, glm::mat4 p_proj, bool p_renderAlphas);
		void MoveEnemies();
		void SpawnEnemies(int numSpawn, std::string enemyFile);
		std::string GetCharacterSelected();
		std::vector<int> PathTowardsClosestHero(int tile);

	private:
		std::list<CharacterUnits> characters;
		std::vector<CharacterUnits> enemies;
		float timeBetween = 1.0f;
		bool targeting = false;
		int currTarget, prevTarget;
		std::string targetName;
		HexGrid* grid;
		float movementTime = 5.0f;
		std::list<CharacterUnits>::iterator it;
};

#endif
