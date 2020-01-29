#ifndef CHARACTERUNITS_H
#define CHARACTERUNITS_H

#include <vector>
#include "BMWModel.h"
#include "ComponentHexPos.h"
#include "Healthbar.h"

class CharacterUnits
{
	public:
		CharacterUnits(std::string bmwFile, std::string shaderFile, int startTile, std::string name, HexGrid* grid, float scale = 1.0f, bool p_inverted = false, glm::vec3 model_color = glm::vec3(1.0, 1.0, 1.0));
		~CharacterUnits();
		void Render(glm::mat4 view, glm::mat4 proj, wolf::RenderFilterType type);
		void Update(float deltaT);
		std::string GetName();
		int GetTile();
		void SetTile(int tile);
		void PlaySound(std::string soundName);
		void SetAnim(std::string animName);
		void Move(std::vector<int> p_path, float p_timeToComplete, bool p_attacking);
		glm::vec3 GetPos();
		void ModifyStats(std::string id, float mult);

		bool getHasMoved();
		void setHasMoved(bool moved);
		bool isMoving();
		bool isAttacking();
		bool isDying();
		void setSelected(bool selected);
		void InitDeath();
		void TakeDamage(std::string p_characterFrom);
		bool cmpf(float a, float b);
		void SetHealthbarVisible(bool isVisible);
		void SetHealthbarPercent(float percent);
		float GetDeathTimer();
		bool InitDamage();
		std::string GetAttacker();

	private:
		std::map<std::string, float> animTimes;
		std::vector<std::string> soundFiles;
		wolf::BMWModel* model;
		int currTile, endTile, prevTile;
		float deathTimer = 0.0f;
		float timeAttacking = 0.0f;
		float timeDamaged = 0.0f;
		std::string name;
		std::string characterAttacking;
		week2::ComponentHexPos pos;
		float scale = 0.0f;
		bool canTakeDamage = false;
		bool initiatingDamage = false;
		bool inverted = false;
		bool changed = false;
		bool dying = false;
		bool damaged = false;
		bool m_attacking = false;
		bool m_hasMoved = false;
		bool m_justMoved = false;
		bool m_isSelected = false;
		bool m_startedAttack = false;
		double m_deltaSum = 0.0;
		bool m_isHealthbarVisible = true;
		Healthbar* m_healthbar;
};

#endif