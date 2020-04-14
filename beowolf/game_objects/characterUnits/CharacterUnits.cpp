#include "CharacterUnits.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"
#define EPSILON_VALUE 0.01


CharacterUnits::CharacterUnits(const std::string p_bmwFile, const std::string p_shaderFile, const int p_startTile, const std::string p_name, const HexGrid* p_grid, const float p_scale, const bool p_inverted, const glm::vec3 model_color)
{
	//Set Animation Wait Times Per character 
	m_animTimes.insert(std::pair<std::string, int>("myLich", 1.0));
	m_animTimes.insert(std::pair<std::string, int>("myChamp", 1.75));
	m_animTimes.insert(std::pair<std::string, int>("mySkeleton", 0.5));
	m_scale = p_scale;
	auto shaders = wolf::ResourceLoader::Instance().getShaders(p_shaderFile);
	auto shadowShaders = wolf::ResourceLoader::Instance().getShaders("shadow_map");
	m_model = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel(p_bmwFile), shaders.first, shaders.second, shadowShaders.first, shadowShaders.second);
	m_model->setModelColor(model_color);
	if (p_inverted)
	{
		m_model->setTransform(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y)) * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));
		m_inverted = p_inverted;
	}
	else
	{
		m_model->setTransform(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y)) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));
	}
	m_currTile = p_startTile;

	m_name = p_name;
	if (m_name == "myGiant")
		m_cooldownHeightAdjustment = 2.5f;

	m_pos.SetGrid(p_grid);
	m_pos.SetPos(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile), p_grid->GetPos().at(p_startTile).y));

	m_healthbar = new Healthbar();
	m_healthbar->SetPos(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile) + 4.0f, p_grid->GetPos().at(p_startTile).y)));

	m_cooldown = new CooldownIndicator();
	m_cooldown->SetPos(glm::translate(glm::vec3(p_grid->GetPos().at(p_startTile).x, p_grid->GetHeights().at(p_startTile) + 5.5f + m_cooldownHeightAdjustment, p_grid->GetPos().at(p_startTile).y)));

	m_grid = p_grid;
}

//Deconstructor
CharacterUnits::~CharacterUnits()
{
	//TODO
}

//Sets Lighting for the unit
void CharacterUnits::SetLighting(glm::vec4 ambLight, glm::vec4 difLight, glm::vec3 lightDir)
{
	m_model->setLightAmbient(ambLight);
	m_model->setLightDiffuse(difLight);
	m_model->setLightDir(lightDir);
}

//Sets Lighting Direction for the unit
void CharacterUnits::SetLightingDir(glm::vec3 dir)
{
	m_model->setLightDir(dir);
}

//Render
void CharacterUnits::Render( glm::mat4 p_view,  glm::mat4 p_proj,  glm::mat4 lightSpaceMatrix,  wolf::RenderFilterType type,  bool shadowPass,  unsigned int depthMapTexture)
{
	//Render Model
	m_model->render(p_view, p_proj, lightSpaceMatrix, type, shadowPass, depthMapTexture, false);

	//Render Healthbar or Cooldown indicator if necessary
	if (m_isHealthbarVisible && type == wolf::RenderFilterTransparent) {
		m_healthbar->Render(p_view, p_proj);
	}
	if (m_isCooldownVisible && type == wolf::RenderFilterTransparent) {
		m_cooldown->Render(p_view, p_proj);
	}

	m_particleProjMatrix = p_proj * p_view;
	m_particleProjMatrixNoBillboard = glm::mat4(glm::mat3(p_proj * glm::rotate(90.0f, glm::vec3(1, 0, 0))));

	//Render any particles as needed
	for (int i = 0; i < m_particleEffects.size(); i++)
		m_particleEffects[i]->Render(m_particleProjMatrix, type);
	for (int i = 0; i < m_particleEffectsNoBillboard.size(); i++)
		m_particleEffectsNoBillboard[i]->Render(m_particleProjMatrix, type);
}

//Update
void CharacterUnits::Update(float deltaT)
{
	glm::vec3 last = m_pos.GetPos();
	m_pos.Update(deltaT);
	glm::vec3 dif = m_pos.GetPos() - last;

	float dir = m_pos.GetDirection();
	if (dif.x != 0 || dif.z != 0)
	{
		dif = glm::normalize(dif);
	}

	//If dying play death animation and if dead set death timer to 100.0f to indicate finished
	if (m_dying && m_deathTimer <= 100.0f)
	{
		if (cmpf(m_deathTimer, 0.0f))
		{
			SetAnim("death_forward");
			SetAnim("death");
		}
		m_deathTimer += deltaT;
		m_model->setTransform(glm::translate(glm::vec3(m_pos.GetPos().x, wolf::Math::lerp(m_pos.GetPos().y, m_pos.GetPos().y - 10.0f, m_deathTimer / 10.0f), m_pos.GetPos().z)) * glm::rotate(dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));
		if (m_model->getIsAnimationRunning() == false)
		{
			m_deathTimer = 100.0f;
		}
	}
	//If not dying check if moving or attacking and set transforms, animations and sounds properly
	else
	{
		if (m_pos.IsMoving() && (dif.x != 0 || dif.z != 0))
		{
			m_model->setTransform(glm::translate(glm::vec3(m_pos.GetPos().x, m_pos.GetPos().y, m_pos.GetPos().z)) * glm::rotate(-dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));
			m_healthbar->SetPos(glm::translate(glm::vec3(m_pos.GetPos().x, m_pos.GetPos().y + 4.0f, m_pos.GetPos().z)));
			m_cooldown->SetPos(glm::translate(glm::vec3(m_pos.GetPos().x, m_pos.GetPos().y + 5.5f + m_cooldownHeightAdjustment, m_pos.GetPos().z)));
		}
		else {
			if (m_attacking)
			{
				if (m_timeAttacking == 0.0f)
				{
					m_startedAttack = true;
					dir = m_pos.GetDirection(m_prevTile, m_endTile);
					m_model->setTransform(glm::translate(glm::vec3(m_pos.GetPos().x, m_pos.GetPos().y, m_pos.GetPos().z)) * glm::rotate(-dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));
					SetAnim("attack");
					SetAnim("attack_begin");
					m_soundEngine->PauseSound("movement1");
					m_soundEngine->UpdateSystem();
				}
				m_timeAttacking += deltaT;
				if (m_timeAttacking >= 1.5f)
				{
					m_attacking = false;
					m_startedAttack = false;
					m_timeAttacking = 0.0f;
					m_model->setModelFilter(glm::vec3(0.7, 0.7, 0.7));
				}
			}
			else if (m_justMoved)
			{
				m_model->setTransform(glm::translate(glm::vec3(m_pos.GetPos().x, m_pos.GetPos().y, m_pos.GetPos().z)) * glm::rotate(-dir, 0.0f, 1.0f, 0.0f) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));
				SetAnim("idle");

				m_soundEngine->PauseSound("movement1");
				m_soundEngine->UpdateSystem();
				m_model->setModelFilter(glm::vec3(0.7, 0.7, 0.7));
			}

			m_justMoved = false;
		}

		//If damaged set additive to make character flash red quickly and play sound
		if (m_damaged)
		{
			std::map<std::string, float>::iterator itm;
			itm = m_animTimes.find(m_characterAttacking.at(0));
			if (itm != m_animTimes.end())
			{
				if (m_timeDamaged >= (itm->second + 0.5))
				{
					m_damaged = false;
					m_model->setModelAdditive(glm::vec3(0, 0, 0));
				}
				else if (m_timeDamaged >= itm->second)
				{
					m_model->setModelAdditive(wolf::Math::lerp(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), (m_timeDamaged - itm->second) / 0.5));
					if (m_canTakeDamage)
					{
						m_initiatingDamage = true;
						m_soundEngine->PlayBasicSound("hit3");
						m_soundEngine->UpdateSystem();
						m_canTakeDamage = false;

						if (m_damageReceivingParticle != "") {
							if (m_damageReceivingBillboarded)
								m_particleEffects.push_back(new Effect(m_damageReceivingParticle));
							else
								m_particleEffectsNoBillboard.push_back(new Effect(m_damageReceivingParticle));
						}
						else {
							if (m_damageReceivingBillboarded)
								m_particleEffects.push_back(new Effect("resources/particles/unit_hit.json"));
							else
								m_particleEffectsNoBillboard.push_back(new Effect("resources/particles/unit_hit.json"));
						}

						if (m_damageReceivingBillboarded)
							m_particleEffects[m_particleEffects.size() - 1]->SetPos(m_pos.GetPos());
						else
							m_particleEffectsNoBillboard[m_particleEffectsNoBillboard.size() - 1]->SetPos(m_pos.GetPos());
					}
				}
				m_timeDamaged += deltaT;
			}
			else
			{
				if (m_timeDamaged >= 1.0)
				{
					m_damaged = false;
					m_model->setModelAdditive(glm::vec3(0, 0, 0));
					m_characterAttacking.clear();

				}
				else if (m_timeDamaged >= 0.5)
				{
					m_model->setModelAdditive(wolf::Math::lerp(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), (m_timeDamaged - 0.5) / 0.5));
					if (m_canTakeDamage)
					{
						m_initiatingDamage = true;
						m_soundEngine->PlayBasicSound("hit3");
						m_soundEngine->UpdateSystem();
						m_canTakeDamage = false;

						if (m_damageReceivingParticle != "") {
							if (m_damageReceivingBillboarded)
								m_particleEffects.push_back(new Effect(m_damageReceivingParticle));
							else
								m_particleEffectsNoBillboard.push_back(new Effect(m_damageReceivingParticle));
						}
						else {
							if (m_damageReceivingBillboarded)
								m_particleEffects.push_back(new Effect("resources/particles/unit_hit.json"));
							else
								m_particleEffectsNoBillboard.push_back(new Effect("resources/particles/unit_hit.json"));
						}

						if (m_damageReceivingBillboarded)
							m_particleEffects[m_particleEffects.size() - 1]->SetPos(m_pos.GetPos());
						else
							m_particleEffectsNoBillboard[m_particleEffectsNoBillboard.size() - 1]->SetPos(m_pos.GetPos() + glm::vec3(0, 1.0, 0));
					}
				}
				m_timeDamaged += deltaT;
			}
		}
	}
	//Update model and cooldown
	m_model->update(deltaT);
	m_cooldown->Update(deltaT);
	
	//Update particle effects
	for (int i = 0; i < m_particleEffects.size(); i++) {
		m_particleEffects[i]->Update(deltaT, glm::mat3(m_particleProjMatrix));
	}
	for (int i = 0; i < m_particleEffectsNoBillboard.size(); i++) {
		m_particleEffectsNoBillboard[i]->Update(deltaT, glm::mat3(m_particleProjMatrixNoBillboard));
	}
}

//Method to compare floats
bool CharacterUnits::cmpf(float a, float b) const
{
	return (fabs(a - b) < EPSILON_VALUE);
}

//Method to get name of unit
const std::string CharacterUnits::GetName() const
{
	return m_name;
}

//Method to get tile unit is on
const int CharacterUnits::GetTile() const
{
	return m_currTile;
}

//Method to set tile of the character
void CharacterUnits::SetTile(int tile, bool updatePositionImmediately)
{
	m_currTile = tile;
	if (updatePositionImmediately) {
		m_model->setTransform(glm::translate(glm::vec3(m_grid->GetPos().at(tile).x, m_grid->GetHeights().at(tile), m_grid->GetPos().at(tile).y)) * glm::scale(glm::vec3(m_scale, m_scale, m_scale)));
		m_pos.SetPos(glm::vec3(m_grid->GetPos().at(tile).x, m_grid->GetHeights().at(tile), m_grid->GetPos().at(tile).y));
		m_healthbar->SetPos(glm::translate(glm::vec3(m_pos.GetPos().x, m_pos.GetPos().y + 4.0f, m_pos.GetPos().z)));
		m_cooldown->SetPos(glm::translate(glm::vec3(m_pos.GetPos().x, m_pos.GetPos().y + 5.5f + m_cooldownHeightAdjustment, m_pos.GetPos().z)));
		Update(0);
	}
}

//method to set the animation of unit
void CharacterUnits::SetAnim(std::string p_animName)
{
	m_model->setAnim(p_animName);
}

//Method to tell unit to move along a path with how long it should take to complete and if it will be attacking at the end
void CharacterUnits::Move(std::vector<int> p_path, float p_timeToComplete, bool p_attacking)
{
	//Play movement sound
	if (m_name.compare("myChamp") == 0 || m_name.compare("myLich") == 0 || m_name.compare("myGiant") == 0)
		m_soundEngine->PlayBasicSound("movement1");
	m_soundEngine->UpdateSystem();
	
	//Set end tile back one if attacking at end if not just move
	if (p_attacking)
	{
		m_attacking = true;
		if (p_path.size() > 1)
		{
			m_endTile = p_path.at(p_path.size() - 1);
			m_prevTile = p_path.at(p_path.size() - 2);
		}
	}
	if (p_path.size() > 1) {
		m_hasMoved = true;
		m_justMoved = true;

		// attempt to walk if run animation doesn't exist
		SetAnim("walk");
		SetAnim("run");
	}
	if (p_path.size() < 1)
	{
		m_hasMoved = true;
		m_justMoved = true;
	}
	//Move character along hexgrid
	m_pos.Move(p_path, p_timeToComplete, p_attacking);
}

//Method to get position of unit
const glm::vec3 CharacterUnits::GetPos() const
{
	return m_pos.GetPos();
}

//Method to check if character has moved
const bool CharacterUnits::getHasMoved() const{
	return m_hasMoved;
}

//Method to set a character movement status
void CharacterUnits::setHasMoved(bool moved) {
	m_hasMoved = moved;

	m_model->setModelFilter(glm::vec3(1.0, 1.0, 1.0));
}

//Method to check if unit is currently moving
bool CharacterUnits::isMoving() const {
	return m_pos.IsMoving();
}

//Method to check if unit currently attacking
bool CharacterUnits::isAttacking() const
{
	return m_startedAttack;
}

//Method to check if unit is currently dying
bool CharacterUnits::isDying() const
{
	return m_dying;
}

//Method to set a unit to be selected
void CharacterUnits::setSelected(bool selected) {
	m_isSelected = selected;

	if (selected)
		m_model->setModelAdditive(glm::vec3(0.5, 0.5, 0.1));
	else
		m_model->setModelAdditive(glm::vec3(0, 0, 0));
}

//Method to initiate unit death
void CharacterUnits::InitDeath()
{
	m_dying = true;
}

//Method to set an indicator to start taking damage
void CharacterUnits::TakeDamage(std::string p_characterFrom, float mult, std::string particleEffectOverride, bool isParticleBillboarded)
{
	m_canTakeDamage = true;
	m_characterAttacking.push_back(p_characterFrom);
	m_timeDamaged = 0.0f;
	m_damaged = true;
	m_damageReceivingMult = mult;
	m_damageReceivingParticle = particleEffectOverride;
	m_damageReceivingBillboarded = isParticleBillboarded;
}

//Method to get the damage multiplier
const float CharacterUnits::GetDamageReceivedMult() const
{
	return m_damageReceivingMult;
}

//Method to set if the health bar is visible
void CharacterUnits::SetHealthbarVisible(bool isVisible)
{
	m_isHealthbarVisible = isVisible;
	m_isCooldownVisible = !isVisible;
}

//Method to set health bar percentage
void CharacterUnits::SetHealthbarPercent(float percent)
{
	m_healthbar->SetThreshold(1.0 - percent);
}

//Method to get death timer
const float CharacterUnits::GetDeathTimer() const
{
	return m_deathTimer;
}

//Method to initiate damage
bool CharacterUnits::InitDamage()
{
	if (m_initiatingDamage)
	{
		m_initiatingDamage = false;
		return true;
	}
	else
		return m_initiatingDamage;
}

//Method to get who is attacking the unit
const std::vector<std::string> CharacterUnits::GetAttacker() const
{
	std::vector<std::string> temp = m_characterAttacking;
	return temp;
}

//Method to get the model of the unit
const wolf::BMWModel* CharacterUnits::GetModel() const
{
	return m_model;
}

//Method to set the sound engine
void CharacterUnits::SetSoundEngine(wolf::SoundEngine* soundEng)
{
	m_soundEngine = soundEng;
}

//Method to start the cooldown of the units special ability
void CharacterUnits::StartCooldown()
{
	m_cooldownCur = m_cooldownMax;
	m_cooldown->SetThreshold(0.0, true);
}

//Method to update the cooldown of the units special ability
void CharacterUnits::UpdateCooldown()
{
	if (m_cooldownCur > 0)
		m_cooldownCur--;

	m_cooldown->SetThreshold(1 - m_cooldownCur / ((float)m_cooldownMax));
}

//Method to get the cooldown of the units special ability
const int CharacterUnits::GetCooldown() const
{
	return m_cooldownCur;
}

//Method to set the cooldown of the units special ability
void CharacterUnits::SetCooldown(int val)
{
	m_cooldownCur = val + 1;
	UpdateCooldown();
}

//Method to start the particles of the healing ability on a unit
void CharacterUnits::HealIndicator()
{
	m_particleEffects.push_back(new Effect("resources/particles/lich_heal.json"));
	m_particleEffects[m_particleEffects.size() - 1]->SetPos(m_pos.GetPos());
}
