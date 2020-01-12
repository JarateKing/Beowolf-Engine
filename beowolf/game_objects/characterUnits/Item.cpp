#include "Item.h"
#include "W_ResourceLoader.h"

Item::Item(std::string p_bmwFile, std::string p_shaderFile, int p_startTile, std::string p_name, HexGrid* p_grid)
{
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second);
	m_pos = glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y);
	model->setTransform(glm::translate(m_pos) * glm::scale(glm::vec3(scale, scale, scale)));
	currTile = p_startTile;
	name = p_name;
	pos.SetGrid(p_grid);
}

Item::~Item()
{
	//TODO
}

void Item::Render(glm::mat4 p_view, glm::mat4 p_proj, bool p_renderAlphas)
{
	model->render(p_view, p_proj, p_renderAlphas);
}

void Item::Update(float deltaT)
{
	m_bobTime += deltaT;
	pos.Update(deltaT);
	
	model->setTransform(glm::translate(glm::vec3(m_pos.x, m_pos.y + sin(m_bobTime) * 0.5f + 1.0f, m_pos.z)) * glm::rotate(m_bobTime * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(scale, scale, scale)));
}

std::string Item::GetName()
{
	return name;
}

int Item::GetTile()
{
	return currTile;
}

void Item::SetTile(int tile)
{
	currTile = tile;
}

void Item::PlaySound(std::string p_soundName)
{
	//TODO
}

void Item::SetAnim(std::string p_animName)
{
	model->setAnim(p_animName);
}

void Item::Move(std::vector<int> p_path, float p_timeToComplete)
{
	pos.Move(p_path, p_timeToComplete);
}
