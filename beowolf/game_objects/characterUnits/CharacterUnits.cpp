#include "CharacterUnits.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"



CharacterUnits::CharacterUnits(std::string p_bmwFile, std::string p_shaderFile, int p_startTile, std::string p_name, HexGrid* p_grid, float p_scale, bool p_inverted, glm::vec3 model_color)
{
	scale = p_scale;
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second);
	model->setModelColor(model_color);
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

	m_healthbar = new Healthbar();
	m_healthbar->SetPos(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile) + 4.0f, p_grid->GetPos().at(p_startTile).y)));
}

CharacterUnits::~CharacterUnits()
{
	//TODO
}

void CharacterUnits::Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type)
{
	model->render(p_view, p_proj, type);

	if (m_isHealthbarVisible && type == wolf::RenderFilterOpaque) {
		m_healthbar->Render(p_view, p_proj);
	}
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

	/*if (dying)
	{
		if (deathTimer == 0.0f)
		{
			SetAnim("death");
		}
		deathTimer += deltaT;
		model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, wolf::Math::lerp(pos.GetPos().y, pos.GetPos().y - 6.0f, deathTimer / 3.0f), pos.GetPos().z)) * glm::rotate(dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
		if (deathTimer / 3.0f >= 1.0f)
		{
			dying = false;
		}
	}

	if (pos.IsMoving() && inverted && !dying)
	{
		model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) * glm::rotate(dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
	}
	else if (pos.IsMoving() && !dying)
	{
		model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));*/
	if (pos.IsMoving() && (dif.x != 0 || dif.z != 0)) {
		if (inverted)
			model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) * glm::rotate(dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
		else
			model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(-dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));

		m_healthbar->SetPos(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y + 4.0f, pos.GetPos().z)));
	}
	else {
		if (m_justMoved) {
			SetAnim("idle");
			model->setModelFilter(glm::vec3(0.7, 0.7, 0.7));
		}

		m_justMoved = false;
	}

	if (m_isSelected) {
		m_deltaSum += deltaT;
		float additiveValue = (sin(m_deltaSum * 6) / 2.0 + 0.5) * 0.1;
		model->setModelAdditive(glm::vec3(0.5 + additiveValue, 0.5 + additiveValue, 0.1 - additiveValue));
	}
	else {
		m_deltaSum = 0;
	}

	model->update(deltaT);
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
	if (p_path.size() > 1) {
		m_hasMoved = true;
		m_justMoved = true;

		// attempt to walk if run animation doesn't exist
		SetAnim("walk");
		SetAnim("run");
	}
	pos.Move(p_path, p_timeToComplete);
}

glm::vec3 CharacterUnits::GetPos()
{
	return pos.GetPos();
}

void CharacterUnits::ModifyStats(std::string id, float mult)
{
	// TODO: make actually modify stats
}

bool CharacterUnits::getHasMoved() {
	return m_hasMoved;
}

void CharacterUnits::setHasMoved(bool moved) {
	m_hasMoved = moved;

	model->setModelFilter(glm::vec3(1.0, 1.0, 1.0));
}

bool CharacterUnits::isMoving() {
	return pos.IsMoving();
}

void CharacterUnits::setSelected(bool selected) {
	m_isSelected = selected;

	if (selected)
		model->setModelAdditive(glm::vec3(0.5, 0.5, 0.1));
	else
		model->setModelAdditive(glm::vec3(0, 0, 0));
}

void CharacterUnits::InitDeath()
{
	dying = true;
}

void CharacterUnits::TakeDamage()
{

}

void CharacterUnits::SetHealthbarVisible(bool isVisible)
{
	m_isHealthbarVisible = isVisible;
}
