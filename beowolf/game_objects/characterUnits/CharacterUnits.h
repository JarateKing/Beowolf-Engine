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
		void Move(std::vector<int> p_path, float timeToComplete);
		glm::vec3 GetPos();
		void ModifyStats(std::string id, float mult);

		bool getHasMoved();
		void setHasMoved(bool moved);
		bool isMoving();
		void setSelected(bool selected);
		void InitDeath();
		void TakeDamage();
		void SetHealthbarVisible(bool isVisible);

	private:
		std::vector<std::string> soundFiles;
		wolf::BMWModel* model;
		int currTile;
		float deathTimer = 0.0f;
		std::string name;
		week2::ComponentHexPos pos;
		float scale = 0.0f;
		bool inverted = false;
		bool changed = false;
		bool dying = false;
		bool m_hasMoved = false;
		bool m_justMoved = false;
		bool m_isSelected = false;
		double m_deltaSum = 0.0;
		bool m_isHealthbarVisible = true;
		Healthbar* m_healthbar;
};

#endif