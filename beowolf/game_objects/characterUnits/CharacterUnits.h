#ifndef CHARACTERUNITS_H
#define CHARACTERUNITS_H

#include <vector>
#include <BMWModel.h>

class CharacterUnits
{
	public:
		CharacterUnits(std::string bmwFile, std::string shaderFile, int startTile, std::string name);
		~CharacterUnits();
		void Render(glm::mat4 view, glm::mat4 proj, bool renderAlphas);
		std::string GetName();
		int GetTile();
		void PlaySound(std::string soundName);
		void SetAnim(std::string animName);

	private:
		std::vector<std::string> soundFiles;
		wolf::BMWModel* model;
		int currTile;
		std::string name;

};

#endif