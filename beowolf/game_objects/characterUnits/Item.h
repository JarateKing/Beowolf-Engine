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
		Item(const std::string& bmwFile, const std::string& shaderFile, const int& startTile, const std::string& name, HexGrid* grid);
		~Item();
		void Render(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& lightSpaceMatrix, const wolf::RenderFilterType& type, const bool& shadowPass, const unsigned int& depthMapTexture);
		void Update(const float& deltaT);
		void SetLighting(const glm::vec4& ambLight, const glm::vec4& difLight, const glm::vec3& lightDir);
		void SetLightingDir(const glm::vec3&);
		std::string GetName();
		int GetTile();
		glm::vec3 GetPos();

	private:
		std::vector<std::string> m_soundFiles;
		wolf::BMWModel* m_model;
		int m_currTile;
		std::string m_name;
		week2::ComponentHexPos m_hexpos;
		float m_scale = 0.25f;

		float m_bobTime;
		glm::vec3 m_pos;
		
		Effect* m_particleGlow;
		glm::mat3 m_storedProj;
};

#endif