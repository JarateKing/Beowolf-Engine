#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <map>
#include "BMWModel.h"
#include "ComponentHexPos.h"
#include "Effect.h"

class Item
{
	public:
		Item(std::string bmwFile, std::string shaderFile, int startTile, std::string jsonFile, std::string name, HexGrid* grid);
		~Item();
		void Render(glm::mat4 view, glm::mat4 proj, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass);
		void Update(float deltaT);
		void SetLighting(glm::vec4 ambLight, glm::vec4 difLight, glm::vec3 lightDir);
		void SetLightingDir(glm::vec3);
		std::string GetName();
		int GetTile();
		glm::vec3 GetPos();
		std::map<std::string, float> GetStats();

	private:
		std::vector<std::string> soundFiles;
		wolf::BMWModel* model;
		int currTile;
		std::string name;
		week2::ComponentHexPos pos;
		float scale = 0.25f;

		float m_bobTime;
		glm::vec3 m_pos;
		std::map<std::string, float> m_statValues;
		
		Effect* m_particleGlow;
		glm::mat3 m_storedProj;
};

#endif