#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <list>
#include "W_Common.h"
#include "CharacterUnits.h"
#include "hexGrid/HexGrid.h"
#include "Item.h"

class CharacterManager
{
	public:
		CharacterManager(HexGrid* p_grid);
		~CharacterManager();
		void Update(float deltaT);
		void Update(int target, float deltaT);
		void Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type);
		void MoveEnemies(int length);
		void SpawnEnemies(int numSpawn, std::string enemyFile);
		void SpawnItem(int pos);
		std::string GetCharacterSelected();
		std::vector<int> PathTowardsClosestHero(int tile);

	private:
		std::list<CharacterUnits> characters;
		std::vector<CharacterUnits> enemies;
		std::vector<Item> items;
		float timeBetween = 1.0f;
		bool targeting = false;
		int currTarget, prevTarget;
		std::string targetName = "";
		HexGrid* grid;
		float movementTime = 0.5f;
		std::vector<int> test;

		std::vector<int> PathTowardsClosestHero(int enemyIndex, int length);
};

#endif
