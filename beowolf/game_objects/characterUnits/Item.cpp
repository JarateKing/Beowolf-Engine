#include "Item.h"
#include "W_ResourceLoader.h"

Item::Item(std::string p_bmwFile, std::string p_shaderFile, int p_startTile, std::string p_name, HexGrid* p_grid)
{
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second);
	model->setTransform(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y)) * glm::scale(glm::vec3(scale, scale, scale)));
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
	pos.Update(deltaT);
	if (pos.IsMoving())
	{
		model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::scale(glm::vec3(scale, scale, scale)));
	}
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
