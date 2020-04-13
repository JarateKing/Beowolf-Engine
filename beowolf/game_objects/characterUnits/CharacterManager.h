#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <list>
#include "W_Common.h"
#include "CharacterUnits.h"
#include "hexGrid/HexGrid.h"
#include "Item.h"
#include "W_Hud.h"
#include "CharacterInfoHub.h"
#include "ScoreTracker.h"
#include "sound/W_SoundEngine.h"
#include "camera/Camera.h"
#include "camera/HexSelector.h"

class CharacterManager
{
	public:
		CharacterManager(HexGrid* p_grid, wolf::Hud* p_hud, std::string savedata = "");
		~CharacterManager();
		bool IsGameOver();
		void Update(int target, float deltaT);
		void Render(glm::mat4 p_view, glm::mat4 p_proj, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTexture);
		void MoveEnemies();
		void SpawnEnemy(int pos, float multiplier);
		void SpawnEnemy(int pos, std::string name);
		void SpawnEnemies();
		void SpawnItem(int pos);
		void SpawnItem(int pos, int type);
		void SetScoreTracker(ScoreTracker* tracker);
		void SetSoundEngine(wolf::SoundEngine* soundEng);
		void SetCamera(Camera* cam);
		void SetGridSelector(HexSelector* selector);
		void SetLightDir(glm::vec3 dir);
		void PrintCharacterTilePos();
		void BlockTiles(std::vector<int> tiles);
		std::string GetCharacterSelected();
		std::vector<CharacterUnits>* getCharacters();
		std::vector<CharacterUnits>* getEnemies();
		std::vector<Item*>* getItems();
		CharacterInfoHub* GetCharacterHub();

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
		wolf::SoundEngine* m_soundEngine;
		int m_enemyCount = 0;
		int m_enemyCap = 3;
		int m_score = 0;
		int m_itemCap = 5;
		int m_charCount = 3;
		Camera* m_cam;
		HexSelector* m_hexSelector;
		float m_cameraTime = 0.0f;
		int m_cameraUnit = 0;
		int m_enemiesSpawnedTotal = 0;
		bool m_isSpecialActive = false;
		bool gameOver = false;

		std::vector<int> PathTowardsClosestHero(int enemyIndex, int length);
		bool IsCharOnTile(int pos);
		void ApplyPathBlocks(std::vector<std::string> toIgnore, bool blockCharacters, bool blockEnemies);
};

#endif
