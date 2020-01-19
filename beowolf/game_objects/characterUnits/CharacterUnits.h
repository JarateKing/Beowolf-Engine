#ifndef CHARACTERUNITS_H
#define CHARACTERUNITS_H

#include <vector>
#include "BMWModel.h"
#include "ComponentHexPos.h"

class CharacterUnits
{
	public:
		CharacterUnits(std::string bmwFile, std::string shaderFile, int startTile, std::string name, HexGrid* grid, float scale, bool inverted);
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

	private:
		std::vector<std::string> soundFiles;
		wolf::BMWModel* model;
		int currTile;
		std::string name;
		week2::ComponentHexPos pos;
		float scale = 0.0f;
		bool inverted = false;

		bool m_hasMoved = false;
};

#endif