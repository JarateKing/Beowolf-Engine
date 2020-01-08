#ifndef CHARACTERUNITS_H
#define CHARACTERUNITS_H

#include <vector>
#include "BMWModel.h"
#include "ComponentHexPos.h"

class CharacterUnits
{
	public:
		CharacterUnits(std::string bmwFile, std::string shaderFile, int startTile, std::string name, HexGrid* grid);
		~CharacterUnits();
		void Render(glm::mat4 view, glm::mat4 proj, bool renderAlphas);
		void Update(float deltaT);
		std::string GetName();
		int GetTile();
		void SetTile(int tile);
		void PlaySound(std::string soundName);
		void SetAnim(std::string animName);
		void Move(std::vector<int> p_path, float timeToComplete);

	private:
		std::vector<std::string> soundFiles;
		wolf::BMWModel* model;
		int currTile;
		std::string name;
		week2::ComponentHexPos pos;
		float scale = 1.5f;

};

#endif