#include "Item.h"
#include "W_ResourceLoader.h"
#include "W_RNG.h"
#include <sstream>
#include "JSON/json.hpp"

const std::vector<std::string> STAT_NAMES = { "Health", "Damage", "Defense" };

Item::Item(std::string p_bmwFile, std::string p_shaderFile, int p_startTile, std::string jsonFile, std::string p_name, HexGrid* p_grid)
{
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second);
	m_pos = glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y);
	model->setTransform(glm::translate(m_pos) * glm::scale(glm::vec3(scale, scale, scale)));
	currTile = p_startTile;
	name = p_name;
	pos.SetGrid(p_grid);
	m_bobTime = wolf::RNG::GetRandom(0.0f, 360.0f);

	std::ifstream jsonIn(wolf::ResourceLoader::Instance().getJSONObject(jsonFile));
	nlohmann::json jsonData;
	if (jsonIn) {
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		jsonData = nlohmann::json::parse(jsonFileData);

		if (jsonData.contains("Stats")) {
			for (int i = 0; i < STAT_NAMES.size(); i++)
				if (jsonData["Stats"].contains(STAT_NAMES[i]))
					m_statValues[STAT_NAMES[i]] = jsonData["Stats"][STAT_NAMES[i]];
		}
	}

	m_particleGlow = new Effect("resources/particles/item_glow.json");
	m_particleGlow->SetPos(m_pos);
}

Item::~Item()
{
	//TODO
}

void Item::Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type)
{
	model->render(p_view, p_proj, type);
	m_particleGlow->Render(p_proj * p_view, type);
	
	m_storedProj = glm::mat3(p_proj * glm::rotate(90.0f, glm::vec3(1, 0, 0)));
}

void Item::Update(float deltaT)
{
	m_bobTime += deltaT;
	pos.Update(deltaT);
	
	model->setTransform(glm::translate(glm::vec3(m_pos.x, m_pos.y + sin(m_bobTime) * 0.5f + 1.0f, m_pos.z)) * glm::rotate(m_bobTime * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(scale, scale, scale)));

	m_particleGlow->Update(deltaT, m_storedProj);
}

std::string Item::GetName()
{
	return name;
}

int Item::GetTile()
{
	return currTile;
}

glm::vec3 Item::GetPos()
{
	return m_pos;
}