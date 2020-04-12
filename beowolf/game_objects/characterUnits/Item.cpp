#include "Item.h"
#include "W_ResourceLoader.h"
#include "W_RNG.h"
#include <sstream>
#include "JSON/json.hpp"

const std::vector<std::string> STAT_NAMES = { "HP", "MaxAttack", "MinAttack", "Defense"};

Item::Item(std::string p_bmwFile, std::string p_shaderFile, int p_startTile, std::string p_name, HexGrid* p_grid)
{
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	auto shadowShaders = wolf::ResourceLoader::Instance().getShaders("shadow_map");
	model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second, shadowShaders.first, shadowShaders.second);
	m_pos = glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y);
	model->setTransform(glm::translate(m_pos) * glm::scale(glm::vec3(scale, scale, scale)));
	currTile = p_startTile;
	name = p_name;
	pos.SetGrid(p_grid);
	m_bobTime = wolf::RNG::GetRandom(0.0f, 360.0f);

	m_particleGlow = new Effect("resources/particles/item_glow.json");
	m_particleGlow->SetPos(m_pos);
}

Item::~Item()
{
	delete m_particleGlow;
}

void Item::Render(glm::mat4 p_view, glm::mat4 p_proj, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTexture)
{
	model->render(p_view, p_proj, lightSpaceMatrix, type, shadowPass, depthMapTexture, false);
	m_particleGlow->Render(p_proj * p_view, type);
	
	m_storedProj = glm::mat3(p_proj * glm::rotate(90.0f, glm::vec3(1, 0, 0)));
}

void Item::SetLighting(glm::vec4 ambLight, glm::vec4 difLight, glm::vec3 lightDir)
{
	model->setLightAmbient(ambLight);
	model->setLightDiffuse(difLight);
	model->setLightDir(lightDir);
}

void Item::SetLightingDir(glm::vec3 lightDir)
{
	model->setLightDir(lightDir);
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
