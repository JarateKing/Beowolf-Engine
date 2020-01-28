#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <list>
//#include <map>
#include "W_Common.h"
#include "CharacterUnits.h"
#include "hexGrid/HexGrid.h"
#include "Item.h"
#include "W_Hud.h"
#include "CharacterInfoHub.h"

class CharacterManager
{
	public:
		CharacterManager(HexGrid* p_grid, wolf::Hud* p_hud);
		~CharacterManager();
		void Update(int target, float deltaT);
		void Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type);
		void MoveEnemies(int length);
		void SpawnEnemies(int numSpawn, std::string enemyFile);
		void SpawnItem(int pos);
		std::string GetCharacterSelected();

		std::vector<CharacterUnits>* getCharacters();
		std::vector<CharacterUnits>* getEnemies();

	private:
		CharacterInfoHub characterIHub;
		std::vector<CharacterUnits> characters;
		std::vector<CharacterUnits> enemies;
		std::vector<Item*> items;
		float timeBetween = 1.0f;
		bool targeting = false;
		bool clickedOnEnemy = false;
		bool enemiesAttacking = false;
		std::string characterMoving;
		std::string targetedEnemy;
		std::string targetedHero;
		std::string enemyMoving;
		int currTarget, prevTarget;
		std::string targetName = "";
		HexGrid* grid;
		float movementTime = 0.5f;
		std::vector<int> test;
		std::map<std::string, std::string> enemyAttacks;
		wolf::Hud* m_hud;

		std::vector<int> PathTowardsClosestHero(int enemyIndex, int length);
};

#endif
