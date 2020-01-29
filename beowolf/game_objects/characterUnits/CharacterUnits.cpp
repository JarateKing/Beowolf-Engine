#include "CharacterUnits.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"
#define EPSILON_VALUE 0.01


CharacterUnits::CharacterUnits(std::string p_bmwFile, std::string p_shaderFile, int p_startTile, std::string p_name, HexGrid* p_grid, float p_scale, bool p_inverted, glm::vec3 model_color)
{
	animTimes.insert(std::pair<std::string, int>("myLich", 1.0));
	animTimes.insert(std::pair<std::string, int>("myChamp", 1.75));
	animTimes.insert(std::pair<std::string, int>("mySkeleton", 0.5));
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

	if (m_isHealthbarVisible && type == wolf::RenderFilterTransparent) {
		m_healthbar->Render(p_view, p_proj);
	}
}

void CharacterUnits::Update(float deltaT)
{
	glm::vec3 last = pos.GetPos();
	pos.Update(deltaT);
	glm::vec3 dif = pos.GetPos() - last;

	float dir = pos.GetDirection();
	if (dif.x != 0 || dif.z != 0)
	{
		dif = glm::normalize(dif);
	}

	if (dying && deathTimer <= 100.0f)
	{
		if (cmpf(deathTimer, 0.0f))
		{
			SetAnim("death_forward");
			SetAnim("death");
		}
		deathTimer += deltaT;
		model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, wolf::Math::lerp(pos.GetPos().y, pos.GetPos().y - 10.0f, deathTimer / 10.0f), pos.GetPos().z)) * glm::rotate(dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
		if (model->getIsAnimationRunning() == false)
		{
			deathTimer = 100.0f;
		}
	}
	else
	{
		if (pos.IsMoving() && (dif.x != 0 || dif.z != 0))
		{
			model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(-dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
			m_healthbar->SetPos(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y + 4.0f, pos.GetPos().z)));
		}
		else {
			if (m_attacking)
			{
				if (timeAttacking == 0.0f)
				{
					m_startedAttack = true;
					dir = pos.GetDirection(prevTile, endTile);
					model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(-dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
					SetAnim("attack");
					SetAnim("attack_begin");
				}
				timeAttacking += deltaT;
				if (timeAttacking >= 1.5f)
				{
					m_attacking = false;
					m_startedAttack = false;
					timeAttacking = 0.0f;
				}
			}
			else if (m_justMoved)
			{
				model->setTransform(glm::translate(glm::vec3(pos.GetPos().x, pos.GetPos().y, pos.GetPos().z)) * glm::rotate(-dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(scale, scale, scale)));
				SetAnim("idle");
				model->setModelFilter(glm::vec3(0.7, 0.7, 0.7));
			}

			m_justMoved = false;
		}

		if (damaged)
		{
			std::map<std::string, float>::iterator itm;
			itm = animTimes.find(characterAttacking);
			if (itm != animTimes.end())
			{
				if (timeDamaged >= (itm->second + 0.5))
				{
					damaged = false;
					model->setModelAdditive(glm::vec3(0, 0, 0));
				}
				else if (timeDamaged >= itm->second)
				{
					model->setModelAdditive(wolf::Math::lerp(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), (timeDamaged - itm->second) / 0.5));
					if (canTakeDamage)
					{
						initiatingDamage = true;
						canTakeDamage = false;
					}
				}
				timeDamaged += deltaT;
			}
			else
			{
				if (timeDamaged >= 1.0)
				{
					damaged = false;
					model->setModelAdditive(glm::vec3(0, 0, 0));
				}
				else if (timeDamaged >= 0.5)
				{
					model->setModelAdditive(wolf::Math::lerp(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), (timeDamaged - 0.5) / 0.5));
					if (canTakeDamage)
					{
						initiatingDamage = true;
						canTakeDamage = false;
					}
				}
				timeDamaged += deltaT;
			}
		}
	}
	model->update(deltaT);
}

bool CharacterUnits::cmpf(float a, float b)
{
	return (fabs(a - b) < EPSILON_VALUE);
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

void CharacterUnits::Move(std::vector<int> p_path, float p_timeToComplete, bool p_attacking)
{
	if (p_attacking)
	{
		m_attacking = true;
		if (p_path.size() > 1)
		{
			endTile = p_path.at(p_path.size() - 1);
			prevTile = p_path.at(p_path.size() - 2);
		}
	}
	if (p_path.size() > 1) {
		m_hasMoved = true;
		m_justMoved = true;

		// attempt to walk if run animation doesn't exist
		SetAnim("walk");
		SetAnim("run");
	}
	pos.Move(p_path, p_timeToComplete, p_attacking);
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

bool CharacterUnits::isAttacking()
{
	return m_startedAttack;
}

bool CharacterUnits::isDying()
{
	return dying;
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

void CharacterUnits::TakeDamage(std::string p_characterFrom)
{
	canTakeDamage = true;
	characterAttacking = p_characterFrom;
	timeDamaged = 0.0f;
	damaged = true;
}

void CharacterUnits::SetHealthbarVisible(bool isVisible)
{
	m_isHealthbarVisible = isVisible;
}

void CharacterUnits::SetHealthbarPercent(float percent)
{
	m_healthbar->SetThreshold(1.0 - percent);
}

float CharacterUnits::GetDeathTimer()
{
	return deathTimer;
}

bool CharacterUnits::InitDamage()
{
	if (initiatingDamage)
	{
		initiatingDamage = false;
		return true;
	}
	else
		return initiatingDamage;
}

std::string CharacterUnits::GetAttacker()
{
	return characterAttacking;
}


