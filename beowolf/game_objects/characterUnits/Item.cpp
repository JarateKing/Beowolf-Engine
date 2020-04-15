#include "Item.h"
#include "W_ResourceLoader.h"
#include "W_RNG.h"
#include <sstream>
#include "JSON/json.hpp"

const std::vector<std::string> STAT_NAMES = { "HP", "MaxAttack", "MinAttack", "Defense"};

Item::Item(const std::string& p_bmwFile, const std::string& p_shaderFile, const int& p_startTile, const std::string& p_name, HexGrid* p_grid)
{
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	auto shadowShaders = wolf::ResourceLoader::Instance().getShaders("shadow_map");
	m_model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second, shadowShaders.first, shadowShaders.second);
	m_pos = glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y);
	m_model->setTransform(glm::translate(m_pos) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));

	m_currTile = p_startTile;
	m_name = p_name;
	m_hexpos.SetGrid(p_grid);
	m_bobTime = wolf::RNG::GetRandom(0.0f, 360.0f);

	// underglow effect
	m_particleGlow = new Effect("resources/particles/item_glow.json");
	m_particleGlow->SetPos(m_pos);
}

Item::~Item()
{
	delete m_particleGlow;
}

void Item::Render(const glm::mat4& p_view, const glm::mat4& p_proj, const glm::mat4& lightSpaceMatrix, const wolf::RenderFilterType& type, const bool& shadowPass, const unsigned int& depthMapTexture)
{
	m_model->render(p_view, p_proj, lightSpaceMatrix, type, shadowPass, depthMapTexture, false);
	m_particleGlow->Render(p_proj * p_view, type);
	
	m_storedProj = glm::mat3(p_proj * glm::rotate(90.0f, glm::vec3(1, 0, 0)));
}

void Item::SetLighting(const glm::vec4& ambLight, const glm::vec4& difLight, const glm::vec3& lightDir)
{
	m_model->setLightAmbient(ambLight);
	m_model->setLightDiffuse(difLight);
	m_model->setLightDir(lightDir);
}

void Item::SetLightingDir(const glm::vec3& lightDir)
{
	m_model->setLightDir(lightDir);
}

void Item::Update(const float& deltaT)
{
	m_bobTime += deltaT;
	m_hexpos.Update(deltaT);
	
	m_model->setTransform(glm::translate(glm::vec3(m_pos.x, m_pos.y + sin(m_bobTime) * 0.5f + 1.0f, m_pos.z)) * glm::rotate(m_bobTime * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));

	m_particleGlow->Update(deltaT, m_storedProj);
}

std::string Item::GetName()
{
	return m_name;
}

int Item::GetTile()
{
	return m_currTile;
}

glm::vec3 Item::GetPos()
{
	return m_pos;
}
