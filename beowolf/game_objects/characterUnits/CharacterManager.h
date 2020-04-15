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
		//Public Methods
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
		const std::string GetCharacterSelected() const;
		std::vector<CharacterUnits>* getCharacters();
		std::vector<CharacterUnits>* getEnemies();
		const std::vector<Item*>* getItems() const;
		CharacterInfoHub* GetCharacterHub();

	private:

		//Private Methods
		std::vector<int> PathTowardsClosestHero(int enemyIndex, int length);
		bool IsCharOnTile(int pos);
		void ApplyPathBlocks(std::vector<std::string> toIgnore, bool blockCharacters, bool blockEnemies);

		//Private Variables
		int m_currTarget, m_prevTarget;
		int m_enemyCount = 0;
		int m_enemyCap = 3;
		int m_score = 0;
		int m_itemCap = 5;
		int m_charCount = 3;
		int m_cameraUnit = 0;
		int m_enemiesSpawnedTotal = 0;
		float m_timeBetween = 1.0f;
		float m_timeDamaging = 0.0f;
		float m_movementTime = 0.5f;
		float m_cameraTime = 0.0f;
		bool m_targeting = false;
		bool m_clickedOnEnemy = false;
		bool m_enemiesAttacking = false;
		bool m_isSpecialActive = false;
		bool m_gameOver = false;
		std::string m_characterMoving;
		std::string m_targetedEnemy;
		std::string m_targetedHero;
		std::string m_enemyMoving;
		std::string m_targetName = "";
		std::map<std::string, std::string> m_enemyAttacks;
		std::vector<int> m_test;
		std::vector<glm::vec3> m_blocked;
		std::vector<CharacterUnits> m_characters;
		std::vector<CharacterUnits> m_enemies;
		std::vector<Item*> m_items;
		wolf::Hud* m_hud = NULL;
		wolf::SoundEngine* m_soundEngine = NULL;
		HexGrid* m_grid = NULL;
		Camera* m_cam = NULL;
		HexSelector* m_hexSelector = NULL;
		ScoreTracker* m_scoreTracker = NULL;
		CharacterInfoHub m_characterIHub;
};

#endif
