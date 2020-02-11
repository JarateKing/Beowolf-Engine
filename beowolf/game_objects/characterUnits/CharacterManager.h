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
#include "ScoreTracker.h"
#include "camera/Camera.h"

class CharacterManager
{
	public:
		CharacterManager(HexGrid* p_grid, wolf::Hud* p_hud);
		~CharacterManager();
		void Update(int target, float deltaT);
		void Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type);
		void SpawnEnemy(int pos, float multiplier);
		void SpawnEnemies();
		void MoveEnemies();
		void SpawnItem(int pos);
		std::string GetCharacterSelected();

		void SetScoreTracker(ScoreTracker* tracker);
		void SetCamera(Camera* cam);

		std::vector<CharacterUnits>* getCharacters();
		std::vector<CharacterUnits>* getEnemies();

		void BlockCharacters();
		void BlockEnemies();
		void BlockTiles(std::vector<int> tiles);

	private:
		CharacterInfoHub characterIHub;
		std::vector<CharacterUnits> characters;
		std::vector<CharacterUnits> enemies;
		std::vector<Item*> items;
		float timeBetween = 1.0f;
		float timeDamaging = 0.0f;
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
		std::vector<glm::vec3> blocked;
		ScoreTracker* m_scoreTracker;
		int m_enemyCount = 0;
		int m_enemyCap = 3;
		int m_score = 0;
		int m_itemCap = 5;
		int m_charCount = 3;
		Camera* m_cam;
		float m_cameraTime = 0.0f;
		int m_cameraUnit = 0;
		int m_enemiesSpawnedTotal = 0;

		std::vector<int> PathTowardsClosestHero(int enemyIndex, int length);
		bool IsCharOnTile(int pos);
		void PreloadCharacterModels();
};

#endif
