#include "CharacterUnits.h"
#include "W_ResourceLoader.h"

CharacterUnits::CharacterUnits(std::string p_bmwFile, std::string p_shaderFile, int p_startTile, std::string p_name, HexGrid* p_grid, float p_scale, bool p_inverted)
{
	scale = p_scale;
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second);
	if (p_inverted)
	{
		model->setTransform(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y)) * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) * glm::scale(glm::vec3(scale, scale, scale)));
		inverted = p_inverted;
	}
	else
	{
		model->setTransform(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y)) * glm::scale(glm::vec3(scale, scale, scale)));
	}
	currTile = p_startTile;
	name = p_name;
	pos.SetGrid(p_grid);
}

CharacterUnits::~CharacterUnits()
{
	//TODO
}

void CharacterUnits::Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type)
{
	model->render(p_view, p_proj, type);
}

void CharacterUnits::Update(float deltaT)
{
	glm::vec3 last = pos.GetPos();
	pos.Update(deltaT);
	glm::vec3 dif = pos.GetPos() - last;

	float dir = 180.0f;
	if (dif.x != 0 || dif.z != 0)
	{
		dif = glm::normalize(dif);
		dir = atan2(dif.z, dif.x) * RAD2DEG;
	}
	dir += 90.0f;

	if (pos.IsMoving() && inverted)
	{
		model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) * glm::rotate(dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
	}
	else if (pos.IsMoving())
	{
		model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::scale(glm::vec3(scale, scale, scale)));
	}
}

std::string CharacterUnits::GetName()
{
	return name;
}

int CharacterUnits::GetTile()
{
	return currTile;
}

void CharacterUnits::SetTile(int tile)
{
	currTile = tile;
}

void CharacterUnits::PlaySound(std::string p_soundName)
{
	//TODO
}

void CharacterUnits::SetAnim(std::string p_animName)
{
	model->setAnim(p_animName);
}

void CharacterUnits::Move(std::vector<int> p_path, float p_timeToComplete)
{
	pos.Move(p_path, p_timeToComplete);
}

glm::vec3 CharacterUnits::GetPos()
{
	return pos.GetPos();
}
