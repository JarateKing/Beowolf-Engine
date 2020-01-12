#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include "BMWModel.h"
#include "ComponentHexPos.h"

class Item
{
	public:
		Item(std::string bmwFile, std::string shaderFile, int startTile, std::string name, HexGrid* grid);
		~Item();
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
		float scale = 0.25f;

		float m_bobTime = 0;
		glm::vec3 m_pos;
};

#endif