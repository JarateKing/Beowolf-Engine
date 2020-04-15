#include "CharacterManager.h"
#include "W_Input.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"
#include <cmath>
#include "camera/HexSelector.h"
#include "StateManager.h"
#include "W_Keybind.h"
#include <sstream>

//Initialize Variables as well as grid and hud
CharacterManager::CharacterManager(HexGrid* p_grid, wolf::Hud* p_hud, std::string savedata)
{
	m_grid = p_grid;
	m_hud = p_hud;

	//Set Starting Positions for Each Hero and Initialize Models
	int sTile1 = m_grid->GetSize() / 2 - 2;
	int sTile2 = m_grid->GetSize() / 2 - 1;
	int sTile3 = m_grid->GetSize() / 2;

	while (m_grid->isMountain(sTile1))
		sTile1 += 1;
	while (m_grid->isMountain(sTile2) || sTile2 == sTile1)
		sTile2 += 1;
	while (m_grid->isMountain(sTile3) || sTile3 == sTile1 || sTile3 == sTile2)
		sTile3 += 1;

	CharacterUnits player1("units/mychamp.bmw", "animatable_untextured", sTile1, "myChamp", p_grid, 5.0, false, glm::vec3(0.1, 0.8, 0.7));
	CharacterUnits player2("units/mygiant.bmw", "animatable_untextured", sTile2, "myGiant", p_grid, 0.05, false, glm::vec3(0.2, 0.7, 0.3));
	CharacterUnits player3("units/mylich.bmw", "animatable_untextured", sTile3, "myLich", p_grid, 0.03, false, glm::vec3(0.75, 0.65, 0.1));

	//Add Character Information to InfoHub
	m_characterIHub.AddCharacter("Characters/hero1.json", "myChamp");
	m_characterIHub.AddCharacter("Characters/hero2.json", "myGiant");
	m_characterIHub.AddCharacter("Characters/hero3.json", "myLich");

	player1.SetHealthbarVisible(false);
	player2.SetHealthbarVisible(false);
	player3.SetHealthbarVisible(false);

	//Set Lighting for all characters
	player1.SetLighting(glm::vec4(0.784f, 0.796f, 0.619f, 1.0f), glm::vec4(0.988f, 1.0f, 0.788f, 1.0f), glm::vec3(-0.5, -0.5, -0.5));
	player2.SetLighting(glm::vec4(0.784f, 0.796f, 0.619f, 1.0f), glm::vec4(0.988f, 1.0f, 0.788f, 1.0f), glm::vec3(-0.5, -0.5, -0.5));
	player3.SetLighting(glm::vec4(0.784f, 0.796f, 0.619f, 1.0f), glm::vec4(0.988f, 1.0f, 0.788f, 1.0f), glm::vec3(-0.5, -0.5, -0.5));

	m_characters.push_back(player1);
	m_characters.push_back(player2);
	m_characters.push_back(player3);

	//Add Enemy and Item Types to InfoHub
	m_characterIHub.AddEnemyType("Characters/enemyLight.json", "mySkeleton");
	m_characterIHub.AddEnemyType("Characters/enemyMedium.json", "myFleshLobber");
	m_characterIHub.AddItemType("Items/sword.json");
	m_characterIHub.AddItemType("Items/shield.json");
	m_characterIHub.AddItemType("Items/potion.json");

	//If theres save data, load save data
	if (savedata != "") {
		std::ifstream jsonIn(savedata);
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		json savejson = json::parse(jsonFileData);

		m_score = savejson["Score"];

		bool charsFound[3] = { false, false, false };
		for (auto character : savejson["Characters"]) {
			std::string name = character["Name"];
			for (int i = 0; i < m_characters.size(); i++) {
				if (name == m_characters[i].GetName()) {
					charsFound[i] = true;
					m_characters[i].SetTile(character["Tile"], true);
					m_characters[i].SetCooldown(character["Cooldown"]);

					for (auto it = character["Stats"].begin(); it != character["Stats"].end(); it++) {
						m_characterIHub.UpdateStat(name, it.key(), it.value());
					}
				}
			}
		}

		for (int i = 2; i >= 0; i--)
			if (!charsFound[i])
				m_characters.erase(m_characters.begin() + i);

		for (auto enemy : savejson["Enemies"]) {
			std::string name = enemy["Name"];
			int pos = enemy["Tile"];
			SpawnEnemy(pos, name);

			for (auto it = enemy["Stats"].begin(); it != enemy["Stats"].end(); it++) {
				m_characterIHub.UpdateStat(name, it.key(), it.value());
			}
		}

		for (auto item : savejson["Items"]) {
			std::cout << "Item here!\n";
			int itemType = 1;
			if (item["Name"] == "Sword")
				itemType = 2;
			if (item["Name"] == "Shield")
				itemType = 3;

			int itemPos = item["Tile"];

			SpawnItem(itemPos, itemType);
		}

		m_enemyCount = m_enemies.size() + m_score;

		// simulate to figure out the proper cap
		for (int i = 0; i <= m_enemyCount; i++) {
			if (i > m_enemyCap * m_enemyCap * 0.5)
				m_enemyCap++;
		}
	}
}

//Deconstructor
CharacterManager::~CharacterManager()
{
}

//Update
void CharacterManager::Update(int p_target, float p_deltaT)
{
	//Move camera to position of characters
	m_cameraTime += p_deltaT;
	if (m_characters.size() > 0) {
		m_cameraUnit %= m_characters.size();
		if (wolf::Keybind::Instance().getBind("switchunit")) {
			if (m_cameraTime < 2.0f) {
				m_cameraUnit = (m_cameraUnit + 1) % m_characters.size();
			}
			else {
				m_cameraUnit = 0;
			}
			
			m_cameraTime = 0.0f;
			m_cam->MoveToView(m_characters[m_cameraUnit].GetPos(), glm::vec3(0, 50 - m_characters[m_cameraUnit].GetPos().y, -40.0f), 0.35f);
		}
	}

	//If clicked on an enemy check to see if a character is attacking and if so do damage
	//Also check if special attack is active
	if (m_clickedOnEnemy)
	{
		for (auto it = m_characters.begin(); it != m_characters.end(); it++)
		{
			if (it->GetName().compare(m_characterMoving) == 0)
			{
				if (it->isAttacking() == true)
				{
					m_clickedOnEnemy = false;

					// special attack
					if (m_isSpecialActive) {
						it->StartCooldown();

						if (it->GetName() == "myGiant") {
							int hitUnitIndex;
							for (int i = 0; i < m_enemies.size(); i++) {
								if (m_enemies[i].GetName() == m_targetedEnemy)
									hitUnitIndex = i;
							}
							for (int i = 0; i < m_enemies.size(); i++) {
								if (i == hitUnitIndex) {
									m_enemies[i].TakeDamage(it->GetName(), 0.75f, "resources/particles/giant_shockwave.json", false);
								}
								else if (glm::distance(glm::vec2(m_enemies[i].GetPos().x, m_enemies[i].GetPos().z), glm::vec2(m_enemies[hitUnitIndex].GetPos().x, m_enemies[hitUnitIndex].GetPos().z)) <= 10.0f) {
									m_enemies[i].TakeDamage(it->GetName(), 0.75f, "resources/particles/blank.json");
								}
							}
						}
						else {
							for (int i = 0; i < m_enemies.size(); i++) {
								if (m_enemies.at(i).GetName().compare(m_targetedEnemy) == 0) {
									m_enemies.at(i).TakeDamage(it->GetName(), 2.5f, "resources/particles/unit_hit_heavy.json");
								}
							}
						}
					}
					// regular attack
					else {
						for (int i = 0; i < m_enemies.size(); i++)
						{
							if (m_enemies.at(i).GetName().compare(m_targetedEnemy) == 0)
							{
								m_enemies.at(i).TakeDamage(it->GetName());
							}
						}
					}

					m_isSpecialActive = false;
				}
			}
		}
	}

	//Checks if enemies are attacking a hero this frame and if so initiates damage
	if (m_enemiesAttacking)
	{
		std::map<std::string, std::string>::iterator it;
		std::vector<std::string> deletions;
		for (it = m_enemyAttacks.begin(); it != m_enemyAttacks.end(); it++)
		{
			for (int i = 0; i < m_enemies.size(); i++)
			{
				if (m_enemies.at(i).isAttacking() == true && m_enemies.at(i).GetName().compare(it->first) == 0)
				{
					deletions.push_back(it->first);
					for (auto itc = m_characters.begin(); itc != m_characters.end(); itc++)
					{
						if (itc->GetName().compare(it->second) == 0)
						{
							itc->TakeDamage(it->first);
						}
					}
				}
			}
		}
		for(int i = 0; i < deletions.size(); i++)
			m_enemyAttacks.erase(deletions.at(i));
		if(m_enemyAttacks.size() == 0)
			m_enemiesAttacking = false;
	}
  
	// TODO - remove this
	static int startpos[] = { 100, 100, 180 };
	static double animtime[] = { 0.0, 0.0, 0.0 };
	// ====

	//Checks to see if damage was initiated and if so complete damage
	//Also initiates camera shake if damage taken
	//If player dies hero death sound is played and character is erased
	for (int i = 0; i < m_characters.size(); i++)
	{
		if (i < m_characters.size())
		{
			if (m_characters.at(i).InitDamage())
			{
				for(int j = 0; j < m_characters.at(i).GetAttacker().size(); j++)
					m_characterIHub.DamageCharacter(m_characters.at(i).GetName(), m_characters.at(i).GetAttacker().at(j), m_characters.at(i).GetDamageReceivedMult());
				m_cam->InitiateShake();
			}

			if (m_characterIHub.GetStat(m_characters.at(i).GetName(), "HP") <= 0.0f)
			{
				m_characters.at(i).InitDeath();
				if (m_characters.size() == 1)
				{
					m_gameOver = true;
				}

				if (m_characters.at(i).GetDeathTimer() >= 99.0f)
				{
					m_soundEngine->PlayBasicSound("hero_death");
					m_soundEngine->UpdateSystem();
					m_characters.erase(m_characters.begin() + i);
				}
			}
		}
	}

	//Checks to see if enemy has taken damage and computes damage taken
	//If enemy killed plays sound and spawns item
	for (int i = 0; i < m_enemies.size(); i++)
	{
		if (i < m_enemies.size())
		{
			if (m_enemies.at(i).InitDamage())
			{
				m_characterIHub.DamageEnemy(m_enemies.at(i).GetName(), m_enemies.at(i).GetAttacker().at(0), m_enemies.at(i).GetDamageReceivedMult());
			}

			if (m_characterIHub.GetStat(m_enemies.at(i).GetName(), "HP") <= 0.0f)
			{
				m_enemies.at(i).InitDeath();

				if (m_enemies.at(i).GetDeathTimer() >= 99.0f)
				{
					m_soundEngine->PlayBasicSound("enemy_death");
					m_soundEngine->UpdateSystem();

					if (m_items.size() < m_itemCap)
						SpawnItem(m_enemies.at(i).GetTile());

					m_enemies.erase(m_enemies.begin() + i);
					
					if (m_scoreTracker != nullptr)
						m_scoreTracker->AddScore(1);
				}
			}
		}
	}

	m_timeBetween += p_deltaT;
	m_currTarget = p_target;

	//Updates all enemies and sets their healthbar percentage
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i].Update(p_deltaT);

		float healthPercent = m_characterIHub.GetStat(m_enemies[i].GetName(), "HP") / m_characterIHub.GetStat(m_enemies[i].GetName(), "Health");
		m_enemies[i].SetHealthbarPercent(healthPercent);
	}

	//Updates all Items
	for (int i = 0; i < m_items.size(); i++)
		m_items[i]->Update(p_deltaT);

	// potentially push up healthbars
	static bool charsAlive[] = { true, true, true };
	if (m_charCount != m_characters.size()) {
		m_charCount = m_characters.size();
		bool charsAliveNow[] = { false, false, false };

		for (int i = 0; i < m_charCount; i++) {
			if (m_characters[i].GetName() == "myChamp")
				charsAliveNow[0] = true;
			if (m_characters[i].GetName() == "myGiant")
				charsAliveNow[1] = true;
			if (m_characters[i].GetName() == "myLich")
				charsAliveNow[2] = true;
		}

		if (charsAlive[0] && !charsAliveNow[0]) {
			for (auto element : m_hud->GetElementsByTag("hpbar1"))
				element->SetVisible(0);
			for (auto element : m_hud->GetElementsByTag("hpbar2"))
				element->SetY(element->GetY() - 60);
			for (auto element : m_hud->GetElementsByTag("hpbar3"))
				element->SetY(element->GetY() - 60);
		}
		if (charsAlive[1] && !charsAliveNow[1]) {
			for (auto element : m_hud->GetElementsByTag("hpbar2"))
				element->SetVisible(0);
			for (auto element : m_hud->GetElementsByTag("hpbar3"))
				element->SetY(element->GetY() - 60);
		}
		if (charsAlive[2] && !charsAliveNow[2]) {
			for (auto element : m_hud->GetElementsByTag("hpbar3"))
				element->SetVisible(0);
		}

		for (int i = 0; i < 3; i++)
			charsAlive[i] = charsAliveNow[i];
	}

	m_hud->GetElement("Unit_1_Indicator")->SetVisible(false);
	m_hud->GetElement("Unit_2_Indicator")->SetVisible(false);
	m_hud->GetElement("Unit_3_Indicator")->SetVisible(false);
	for (auto it : m_hud->GetElementsByTag("uihoverpanel"))
		it->SetVisible(false);
	m_hud->GetElement("HoverPanelBG")->SetH(600);

	//Displays Hover Information if Enemy being Hovered Over
	for (auto it = m_enemies.begin(); it != m_enemies.end(); it++) {
		if (p_target == it->GetTile() && (StateManager::getInstance().GetState() == State::GamestatePlayerTurn || StateManager::getInstance().GetState() == State::GamestateEnemyTurn)) {
			for (auto it : m_hud->GetElementsByTag("uihoverpanel"))
				it->SetVisible(true);
			for (auto it : m_hud->GetElementsByTag("uihoverpanelitem"))
				it->SetVisible(false);

			std::string currentUnitName = "Zombie";
			if (it->GetName().find("mySkeleton") == 0)
				currentUnitName = "Skeleton";

			m_hud->SetVar("HoverName", currentUnitName);
			m_hud->SetVar("HoverDescription", m_characterIHub.GetDescription(it->GetName()));
			m_hud->SetVar("HoverHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "HP")));
			m_hud->SetVar("HoverMaxHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Health")));
			m_hud->SetVar("HoverAttackStat", std::to_string((int)((m_characterIHub.GetStat(it->GetName(), "MaxAttack") + m_characterIHub.GetStat(it->GetName(), "MinAttack")) / 2.0f)));
			m_hud->SetVar("HoverDefenseStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Defense")));
			m_hud->SetVar("HoverSpeedStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "MaxMovement")));
		}
	}

	//Displays Hover Information if Item being Hovered Over
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if (p_target == (*it)->GetTile() && (StateManager::getInstance().GetState() == State::GamestatePlayerTurn || StateManager::getInstance().GetState() == State::GamestateEnemyTurn)) {
			for (auto it : m_hud->GetElementsByTag("uihoverpanel"))
				it->SetVisible(true);
			for (auto it : m_hud->GetElementsByTag("uihoverpanelstat"))
				it->SetVisible(false);
			m_hud->GetElement("HoverPanelBG")->SetH(300);

			m_hud->SetVar("HoverName", (*it)->GetName());
			m_hud->SetVar("HoverDescription", m_characterIHub.GetDescription((*it)->GetName()));

			if ((*it)->GetName() == "Potion") {
				m_hud->SetVar("HoverItemStatName", "Health");
				m_hud->SetVar("HoverItemStatValue", std::to_string((int)m_characterIHub.GetStat((*it)->GetName(), "HP")));
			}
			else if ((*it)->GetName() == "Sword") {
				m_hud->SetVar("HoverItemStatName", "Attack");
				m_hud->SetVar("HoverItemStatValue", std::to_string((int)m_characterIHub.GetStat((*it)->GetName(), "MaxAttack")));
			}
			else {
				m_hud->SetVar("HoverItemStatName", "Defense");
				m_hud->SetVar("HoverItemStatValue", std::to_string((int)m_characterIHub.GetStat((*it)->GetName(), "Defense")));
			}
		}
	}

	//Update Heroes and check for target
	for (auto it = m_characters.begin(); it != m_characters.end(); it++)
	{
		it->Update(p_deltaT);

		// check for items
		for (int i = 0; i < m_items.size(); i++) {
			if (glm::length(it->GetPos() - m_items[i]->GetPos()) < 0.25) {

				m_characterIHub.GivePlayerItem(it->GetName(), m_items[i]->GetName());
				m_soundEngine->PlayBasicSound("item_pickup");
				m_soundEngine->UpdateSystem();
				delete m_items[i];
				m_items.erase(m_items.begin() + i);
				i--;
			}
		}

		// do hud specific stuff
		if (it->GetName() == "myChamp") {
			if (p_target == it->GetTile()) {
				m_hud->GetElement("Unit_1_Indicator")->SetVisible(true);

				if (StateManager::getInstance().GetState() == State::GamestatePlayerTurn || StateManager::getInstance().GetState() == State::GamestateEnemyTurn) {
					for (auto it : m_hud->GetElementsByTag("uihoverpanel"))
						it->SetVisible(true);
					for (auto it : m_hud->GetElementsByTag("uihoverpanelitem"))
						it->SetVisible(false);

					m_hud->SetVar("HoverName", "Knight");
					m_hud->SetVar("HoverDescription", m_characterIHub.GetDescription(it->GetName()));
					m_hud->SetVar("HoverHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "HP")));
					m_hud->SetVar("HoverMaxHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Health")));
					m_hud->SetVar("HoverAttackStat", std::to_string((int)((m_characterIHub.GetStat(it->GetName(), "MaxAttack") + m_characterIHub.GetStat(it->GetName(), "MinAttack")) / 2.0f)));
					m_hud->SetVar("HoverDefenseStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Defense")));
					m_hud->SetVar("HoverSpeedStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "MaxMovement")));
				}
			}

			float health = m_characterIHub.GetStat(it->GetName(), "HP");
			float maxhealth = m_characterIHub.GetStat(it->GetName(), "Health");
			m_hud->SetVar("UnitHealth1", std::to_string((int)std::ceil(health)));
			m_hud->SetVar("UnitHealthMax1", std::to_string((int)std::ceil(maxhealth)));
			m_hud->GetElement("healthbar_unit_1")->SetW(314.0 * health / maxhealth);
			if (startpos[0] != health) {
				animtime[0] += p_deltaT * 1.5;

				m_hud->GetElement("healthbar_unit_1_highlight")->SetW(314.0 * wolf::Math::lerp(startpos[0], health, wolf::Math::easeIn(std::min(1.0, animtime[0]))) / maxhealth);
				if (animtime[0] >= 1.0) {
					startpos[0] = health;
					animtime[0] = 0.0;
				}
			}
		}
		else if (it->GetName() == "myGiant") {
			if (p_target == it->GetTile()) {
				m_hud->GetElement("Unit_2_Indicator")->SetVisible(true);

				if (StateManager::getInstance().GetState() == State::GamestatePlayerTurn || StateManager::getInstance().GetState() == State::GamestateEnemyTurn) {
					for (auto it : m_hud->GetElementsByTag("uihoverpanel"))
						it->SetVisible(true);
					for (auto it : m_hud->GetElementsByTag("uihoverpanelitem"))
						it->SetVisible(false);

					m_hud->SetVar("HoverName", "Giant");
					m_hud->SetVar("HoverDescription", m_characterIHub.GetDescription(it->GetName()));
					m_hud->SetVar("HoverHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "HP")));
					m_hud->SetVar("HoverMaxHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Health")));
					m_hud->SetVar("HoverAttackStat", std::to_string((int)((m_characterIHub.GetStat(it->GetName(), "MaxAttack") + m_characterIHub.GetStat(it->GetName(), "MinAttack")) / 2.0f)));
					m_hud->SetVar("HoverDefenseStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Defense")));
					m_hud->SetVar("HoverSpeedStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "MaxMovement")));
				}
			}

			float health = m_characterIHub.GetStat(it->GetName(), "HP");
			float maxhealth = m_characterIHub.GetStat(it->GetName(), "Health");
			m_hud->SetVar("UnitHealth2", std::to_string((int)std::ceil(health)));
			m_hud->SetVar("UnitHealthMax2", std::to_string((int)std::ceil(maxhealth)));
			m_hud->GetElement("healthbar_unit_2")->SetW(314.0 * health / maxhealth);
			if (startpos[1] != health) {
				animtime[1] += p_deltaT * 1.5;

				m_hud->GetElement("healthbar_unit_2_highlight")->SetW(314.0 * wolf::Math::lerp(startpos[1], health, wolf::Math::easeIn(std::min(1.0, animtime[1]))) / maxhealth);
				if (animtime[1] >= 1.0) {
					startpos[1] = health;
					animtime[1] = 0.0;
				}
			}
		}
		else if (it->GetName() == "myLich") {
			if (p_target == it->GetTile()) {
				m_hud->GetElement("Unit_3_Indicator")->SetVisible(true);

				if (StateManager::getInstance().GetState() == State::GamestatePlayerTurn || StateManager::getInstance().GetState() == State::GamestateEnemyTurn) {
					for (auto it : m_hud->GetElementsByTag("uihoverpanel"))
						it->SetVisible(true);
					for (auto it : m_hud->GetElementsByTag("uihoverpanelitem"))
						it->SetVisible(false);

					m_hud->SetVar("HoverName", "Lich");
					m_hud->SetVar("HoverDescription", m_characterIHub.GetDescription(it->GetName()));
					m_hud->SetVar("HoverHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "HP")));
					m_hud->SetVar("HoverMaxHealth", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Health")));
					m_hud->SetVar("HoverAttackStat", std::to_string((int)((m_characterIHub.GetStat(it->GetName(), "MaxAttack") + m_characterIHub.GetStat(it->GetName(), "MinAttack")) / 2.0f)));
					m_hud->SetVar("HoverDefenseStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "Defense")));
					m_hud->SetVar("HoverSpeedStat", std::to_string((int)m_characterIHub.GetStat(it->GetName(), "MaxMovement")));
				}
			}

			float health = m_characterIHub.GetStat(it->GetName(), "HP");
			float maxhealth = m_characterIHub.GetStat(it->GetName(), "Health");
			m_hud->SetVar("UnitHealth3", std::to_string((int)std::ceil(health)));
			m_hud->SetVar("UnitHealthMax3", std::to_string((int)std::ceil(maxhealth)));
			m_hud->GetElement("healthbar_unit_3")->SetW(314.0 * health / maxhealth);
			if (startpos[2] != health) {
				animtime[2] += p_deltaT * 1.5;

				m_hud->GetElement("healthbar_unit_3_highlight")->SetW(314.0 * wolf::Math::lerp(startpos[2], health, wolf::Math::easeIn(std::min(1.0, animtime[2]))) / maxhealth);
				if (animtime[2] >= 1.0) {
					startpos[2] = health;
					animtime[2] = 0.0;
				}
			}
		}
	}

	//Check if mouse pressed on top of hero
	if (wolf::Keybind::Instance().getBind("select") && m_targeting == false && m_timeBetween >= 0.2f)
	{		
		m_prevTarget = m_currTarget;
		for (auto it = m_characters.begin(); it != m_characters.end(); it++)
		{
			if (it->GetTile() == m_currTarget && !it->getHasMoved())
			{
				std::vector<std::string> toIgnore;
				toIgnore.push_back(it->GetName());
				ApplyPathBlocks(toIgnore, true, false);

				m_targetName = it->GetName();
				m_grid->StartTargeting(m_currTarget, m_characterIHub.GetStat(it->GetName(), "MaxMovement") + 1);
				m_targeting = true;
				m_timeBetween = 0.0f;
				it->setSelected(true);
				it = m_characters.end();
				it--;
			}
			else
			{
				m_grid->StopTargeting();
				m_targeting = false;
				m_timeBetween = 1.0f;
			}
		}
	}

	// handle special moves
	if (wolf::Keybind::Instance().getBind("togglespecial") && m_targeting == true) {
		for (int i = 0; i < m_characters.size(); i++) {
			if (m_characters[i].GetName() == m_targetName && m_characters[i].GetCooldown() == 0) {
				// special case with the lich
				if (m_characters[i].GetName() == "myLich") {
					m_characters[i].StartCooldown();
					for (int j = 0; j < m_characters.size(); j++) {
						m_characterIHub.UpdateStat(m_characters[j].GetName(), "HP", std::min(m_characterIHub.GetStat(m_characters[j].GetName(), "HP") + 100, m_characterIHub.GetStat(m_characters[j].GetName(), "Health")));
						m_characters[j].HealIndicator();
					}
				}
				else {
					m_isSpecialActive = !m_isSpecialActive;
				}
			}
		}
	}

	//check if mouse pressed for second time away from hero
	if (wolf::Keybind::Instance().getBind("select") && m_targeting == true && m_timeBetween >= 0.2f)
	{
		bool heroPositionedOnTile = false;
		//bool withinTarget = false;

		for (int i = 0; i < m_characters.size(); i++)
		{
			for (auto it = m_characters.begin(); it != m_characters.end(); it++)
			{
				if (it->GetTile() == m_currTarget)
					heroPositionedOnTile = true;					
			}
		}
		//Check if not clicking on hero position or double clicking on hero
		if (!heroPositionedOnTile)
		{
			bool targetingEnemy = false;
			std::string targetEnemy = "";
			for (int i = 0; i < m_enemies.size(); i++)
			{
				if (m_enemies.at(i).GetTile() == m_currTarget)
				{
					targetEnemy = m_enemies.at(i).GetName();
					targetingEnemy = true;
				}
			}
			//Check if targeting an Enemy
			if (targetingEnemy)
			{
				for (auto it = m_characters.begin(); it != m_characters.end(); it++)
				{
					if (it->GetName().compare(m_targetName) == 0)
					{
						std::vector<std::string> toIgnore;
						toIgnore.push_back(m_targetName);
						toIgnore.push_back(targetEnemy);
						ApplyPathBlocks(toIgnore, true, true);

						std::vector<int> path = m_grid->GetPathway(m_prevTarget, m_currTarget);

						int pSize = 0;
						for (int i = 1; i < path.size(); i++)
						{
							if (m_grid->isDesert(path.at(i)))
								pSize += 2;
							else
								pSize += 1;
						}
						int mMove = (int)m_characterIHub.GetStat(it->GetName(), "MaxMovement");

						if ((path.size() > 0) && (pSize <= mMove))
						{
							if (pSize <= mMove)
							{
								m_targeting = false;
								m_timeBetween = 0.0f;
								it->setSelected(false);
								m_grid->StopTargeting();
								it->Move(path, m_movementTime, true);
								if (path.size() > 1)
									it->SetTile(path.at(path.size() - 2));
								else
								{
									int pop = path.back();
									path.pop_back();
									path.push_back(p_target);
									path.push_back(pop);
									it->SetTile(path.at(path.size() - 2));
								}
								m_clickedOnEnemy = true;
								m_characterMoving = it->GetName();
								m_targetedEnemy = targetEnemy;
								it = m_characters.end();
								it--;
							}
						}
					}
				}
			}
			else
			//If not targeting enemy
			{
				m_isSpecialActive = false;
				for (auto it = m_characters.begin(); it != m_characters.end(); it++)
				{
					if (it->GetName().compare(m_targetName) == 0)
					{
						std::vector<std::string> toIgnore;
						toIgnore.push_back(m_targetName);
						ApplyPathBlocks(toIgnore, true, true);

						std::vector<int> path = m_grid->GetPathway(m_prevTarget, m_currTarget);

						int pSize = 0;
						for (int i = 1; i < path.size(); i++)
						{
							if (m_grid->isDesert(path.at(i)))
								pSize += 2;
							else
								pSize += 1;
						}
						int mMove = (int)m_characterIHub.GetStat(it->GetName(), "MaxMovement");


						if ((path.size() > 0) && (pSize <= mMove))
						{
							m_targeting = false;
							m_timeBetween = 0.0f;
							it->setSelected(false);
							m_grid->StopTargeting();
							it->Move(path, m_movementTime, false);
							it->SetTile(path.at(path.size() - 1));
							it = m_characters.end();
							it--;
						}
					}
				}
			}
		}
	}
}

//Render
void CharacterManager::Render(glm::mat4 p_view, glm::mat4 p_proj, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTexture)
{ 
	//Render all Characters
	for (auto it = m_characters.begin(); it != m_characters.end(); it++)
	{
		it->Render(p_view, p_proj, lightSpaceMatrix, type, shadowPass, depthMapTexture);
	}
	
	//Render all Enemies
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies.at(i).Render(p_view, p_proj, lightSpaceMatrix, type, shadowPass, depthMapTexture);
	}

	//Render all Items
	for (int i = 0; i < m_items.size(); i++)
		m_items[i]->Render(p_view, p_proj, lightSpaceMatrix, type, shadowPass, depthMapTexture);
}

//Complete Enemy Movements
void CharacterManager::MoveEnemies()
{
	bool attacking = false;
	//For each Enemy get their max movement distance and find closest hero
	//Then find the path to that hero and move max distance towards that hero
	for (int i = 0; i < m_enemies.size(); i++)
	{
		int enemyMovement = (int)m_characterIHub.GetStat(m_enemies.at(i).GetName(), "MaxMovement");
		std::vector<int> pathToHero = PathTowardsClosestHero(i, enemyMovement);
		for (auto it = m_characters.begin(); it != m_characters.end(); it++)
		{
			for (int j = 0; j < pathToHero.size(); j++)
			{
				if (pathToHero.at(j) == it->GetTile())
				{
					attacking = true;
					m_enemyAttacks.insert(std::pair<std::string, std::string>(m_enemies.at(i).GetName(), it->GetName()));
					m_enemiesAttacking = true;
				}
			}

		}

		std::cout << pathToHero.size() << std::endl;
		if (pathToHero.size() > 0)
		{
			if (attacking)
			{
				m_enemies.at(i).Move(pathToHero, m_movementTime, true);
				m_enemies.at(i).SetTile(pathToHero.at(pathToHero.size() - 2));
			}
			else
			{
				m_enemies.at(i).Move(pathToHero, m_movementTime, false);
				m_enemies.at(i).SetTile(pathToHero.at(pathToHero.size() - 1));
			}
		}
		//Makes sure game doesnt crash if hero surrounded or no moves available
		else if (pathToHero.size() == 0)
		{
			pathToHero.clear();
			pathToHero.push_back(m_enemies.at(i).GetTile());
			m_enemies.at(i).Move(pathToHero, m_movementTime, false);
			m_enemies.at(i).setHasMoved(true);
		}

		attacking = false;
	}
}

//Method to spawn a random enemy on a position with a multiplier for their health, attack and defense
void CharacterManager::SpawnEnemy(int pos, float multiplier)
{
	m_enemiesSpawnedTotal++;

	m_enemyCount++;
	if (m_enemyCount > m_enemyCap * m_enemyCap * 0.5)
		m_enemyCap++;

	int unitType = wolf::RNG::GetRandom(0, 1);

	CharacterUnits Enemy((unitType)?"units/myskeleton.bmw":"units/myfleshlobber.bmw", "animatable_untextured", pos, ((unitType)?"mySkeleton":"myFleshLobber")+std::to_string(m_enemyCount), m_grid, (unitType)?0.03:0.07, false, glm::vec3(0.7, 0.1, 0));
	m_characterIHub.AddEnemyType((unitType)?"Characters/enemyLight.json":"Characters/enemyMedium.json", Enemy.GetName());
	Enemy.SetSoundEngine(m_soundEngine);
	Enemy.SetLighting(glm::vec4(0.784f, 0.796f, 0.619f, 1.0f), glm::vec4(0.988f, 1.0f, 0.788f, 1.0f), glm::vec3(-0.5, -0.5, -0.5));

	m_characterIHub.UpdateStat(Enemy.GetName(), "HP", m_characterIHub.GetStat(Enemy.GetName(), "HP") * multiplier);
	m_characterIHub.UpdateStat(Enemy.GetName(), "Health", m_characterIHub.GetStat(Enemy.GetName(), "Health") * multiplier);
	m_characterIHub.UpdateStat(Enemy.GetName(), "MaxAttack", m_characterIHub.GetStat(Enemy.GetName(), "MaxAttack") * multiplier);
	m_characterIHub.UpdateStat(Enemy.GetName(), "Defense", m_characterIHub.GetStat(Enemy.GetName(), "Defense") * multiplier);

	m_enemies.push_back(Enemy);
}

//Method to spawn a specific enemy on a position
void CharacterManager::SpawnEnemy(int pos, std::string name)
{
	int unitType = 0;
	if (name.find("Skeleton") != std::string::npos)
		unitType = 1;

	CharacterUnits Enemy((unitType) ? "units/myskeleton.bmw" : "units/myfleshlobber.bmw", "animatable_untextured", pos, name, m_grid, (unitType) ? 0.03 : 0.07, false, glm::vec3(0.7, 0.1, 0));
	m_characterIHub.AddEnemyType((unitType) ? "Characters/enemyLight.json" : "Characters/enemyMedium.json", name);
	Enemy.SetSoundEngine(m_soundEngine);
	Enemy.SetLighting(glm::vec4(0.784f, 0.796f, 0.619f, 1.0f), glm::vec4(0.988f, 1.0f, 0.788f, 1.0f), glm::vec3(-0.5, -0.5, -0.5));

	m_enemies.push_back(Enemy);
}

//Method to Spawn multiple enemies and find positions to spawn them
//Spawns up to enemy cap
void CharacterManager::SpawnEnemies()
{
	while (m_enemies.size() < m_enemyCap) {
		int iters = 0;
		int pos = m_grid->GetRandomBorder();
		while (iters++ < 100 && (m_grid->isMountain(pos) || IsCharOnTile(pos)))
			pos = m_grid->GetRandomBorder();

		if (iters >= 100)
			return;
		
		float multiply = m_enemiesSpawnedTotal / 100.0f + 1.0f;

		SpawnEnemy(pos, multiply);
	}
}

//Method to spawn a random item given a position
void CharacterManager::SpawnItem(int pos)
{
	int itemType = wolf::RNG::GetRandom(1, 3);
	SpawnItem(pos, itemType);
}

//Method to spawn a specific item given a position
void CharacterManager::SpawnItem(int pos, int type)
{
	if (type == 1)
		m_items.push_back(new Item("potion.bmw", "lit_textured", pos, "Potion", m_grid));
	else if (type == 2)
		m_items.push_back(new Item("sword1.bmw", "lit_textured", pos, "Sword", m_grid));
	else
		m_items.push_back(new Item("shield.bmw", "lit_textured", pos, "Shield", m_grid));

	m_items.back()->SetLighting(glm::vec4(0.784f, 0.796f, 0.619f, 1.0f), glm::vec4(0.988f, 1.0f, 0.788f, 1.0f), glm::vec3(-0.5, -0.5, -0.5));
}

//Method to get a path towards the closest hero
std::vector<int> CharacterManager::PathTowardsClosestHero(int enemyIndex, int length)
{
	std::vector<int> path;
	std::vector<int> endPath;
	int closest = -1;
	int distance = 10000;
	int current = 0;

	m_grid->ClearBlocks();
	std::vector<int> tilesBlocked;

	//Makes sure to block other enemy tiles
	for (int i = 0; i < m_enemies.size(); i++)
	{
		if (m_enemies.at(i).GetName().compare(m_enemies.at(enemyIndex).GetName()) != 0)
			tilesBlocked.push_back(m_enemies.at(i).GetTile());
	}

	BlockTiles(tilesBlocked);

	//For each character check if pathsize is less than current lowest and return lowest
	for (auto it = m_characters.begin(); it != m_characters.end(); it++)
	{
		path = m_grid->GetPathway(m_enemies.at(enemyIndex).GetTile(), it->GetTile());
		if (path.size() < distance && path.size() != 0)
		{
			distance = path.size();
			closest = current;
			endPath = path;
		}
		current++;
	}
	if(endPath.size() > length + 1)
		endPath.resize(length + 1);
	return endPath;
}

//Method to get current selected character
const std::string CharacterManager::GetCharacterSelected() const 
{
	return m_targetName;
}

//Method to get all characters
std::vector<CharacterUnits>* CharacterManager::getCharacters()
{
	return &m_characters;
}

//Method to get all enemies
std::vector<CharacterUnits>* CharacterManager::getEnemies()
{
	return &m_enemies;
}

//Method to get all items
const std::vector<Item*>* CharacterManager::getItems() const
{
	return &m_items;
}

//Method to get Character Hub
CharacterInfoHub* CharacterManager::GetCharacterHub()
{
	return &m_characterIHub;
}

//Method to block a set of tiles from pathfinding
void CharacterManager::BlockTiles(std::vector<int> tiles)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		m_grid->BlockNodePositions(glm::vec3(m_grid->GetPos().at(tiles.at(i)).x, 0.0f, m_grid->GetPos().at(tiles.at(i)).y));
	}
}

//Method to set score tracker
void CharacterManager::SetScoreTracker(ScoreTracker* tracker)
{
	m_scoreTracker = tracker;
	if (m_score != 0) {
		m_scoreTracker->SetScore(m_score);
		m_score = 0;
	}
}

//Method to Set Camera
void CharacterManager::SetCamera(Camera* cam)
{
	m_cam = cam;
}

//Method to Set Grid Selector indicator
void CharacterManager::SetGridSelector(HexSelector* selector)
{
	m_hexSelector = selector;
}

//Method to check if a character is on a current tile
bool CharacterManager::IsCharOnTile(int pos) {
	for (int i = 0; i < m_characters.size(); i++)
		if (m_characters[i].GetTile() == pos)
			return true;

	for (int i = 0; i < m_enemies.size(); i++)
		if (m_enemies[i].GetTile() == pos)
			return true;

	return false;
}

//Method to set the current Sound Engine
void CharacterManager::SetSoundEngine(wolf::SoundEngine* soundEng)
{
	m_soundEngine = soundEng;
	for (int i = 0; i < m_characters.size(); i++)
	{
		m_characters.at(i).SetSoundEngine(m_soundEngine);
	}
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies.at(i).SetSoundEngine(m_soundEngine);
	}
}

//Method to apply current path blocks with a check to see if characters or enemies should be blocked
void CharacterManager::ApplyPathBlocks(std::vector<std::string> toIgnore, bool blockCharacters, bool blockEnemies)
{
	m_grid->ClearBlocks();

	std::vector<int> tilesBlocked;

	if (blockEnemies)
		for (int i = 0; i < m_enemies.size(); i++)
			if (std::find(toIgnore.begin(), toIgnore.end(), m_enemies[i].GetName()) == toIgnore.end())
				tilesBlocked.push_back(m_enemies.at(i).GetTile());

	if (blockCharacters)
		for (int i = 0; i < m_characters.size(); i++)
			if (std::find(toIgnore.begin(), toIgnore.end(), m_characters[i].GetName()) == toIgnore.end())
				tilesBlocked.push_back(m_characters.at(i).GetTile());

	BlockTiles(tilesBlocked);
}

//Debug method to check character tile positions
void CharacterManager::PrintCharacterTilePos()
{
	std::cout << "================================\n\n";
	for (int i = 0; i < m_characters.size(); i++)
	{
		std::cout << m_characters.at(i).GetTile() << ", (" << m_characters.at(i).GetPos().x << ", " << m_characters.at(i).GetPos().y << ") \n";
	}

	for (int i = 0; i < m_enemies.size(); i++)
	{
		std::cout << m_enemies.at(i).GetTile() << ", (" << m_enemies.at(i).GetPos().x << ", " << m_enemies.at(i).GetPos().y << ") \n";
	}
	std::cout << "================================\n\n";
}

//Method to set light direction for all characters, enemies and items
void CharacterManager::SetLightDir(glm::vec3 dir)
{
	for (int i = 0; i < m_characters.size(); i++)
	{
		m_characters.at(i).SetLightingDir(dir);
	}

	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies.at(i).SetLightingDir(dir);
	}

	for (int i = 0; i < m_items.size(); i++)
	{
		m_items.at(i)->SetLightingDir(dir);
	}
}

//Method to check if game is over
bool CharacterManager::IsGameOver()
{
	return m_gameOver;
}
