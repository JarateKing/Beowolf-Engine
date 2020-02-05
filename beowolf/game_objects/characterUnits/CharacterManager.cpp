#include "CharacterManager.h"
#include "W_Input.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"
#include <cmath>

CharacterManager::CharacterManager(HexGrid* p_grid, wolf::Hud* p_hud)
{
	grid = p_grid;
	m_hud = p_hud;
	PreloadCharacterModels();

	CharacterUnits player1("units/mychamp.bmw", "animatable_untextured", 107, "myChamp", p_grid, 5.0, false, glm::vec3(0.1, 0.8, 0.7));
	characterIHub.AddCharacter("Characters/hero1.json", "myChamp");
	CharacterUnits player2("units/mygiant.bmw", "animatable_untextured", 108, "myGiant", p_grid, 0.05, false, glm::vec3(0.2, 0.7, 0.3));
	characterIHub.AddCharacter("Characters/hero2.json", "myGiant");
	CharacterUnits player3("units/mylich.bmw", "animatable_untextured", 109, "myLich", p_grid, 0.03, false, glm::vec3(0.75, 0.65, 0.1));
	characterIHub.AddCharacter("Characters/hero3.json", "myLich");
	
	player1.SetHealthbarVisible(false);
	player2.SetHealthbarVisible(false);
	player3.SetHealthbarVisible(false);

	characters.push_back(player1);
	characters.push_back(player2);
	characters.push_back(player3);

	characterIHub.AddEnemyType("Characters/enemyLight.json", "mySkeleton");
	characterIHub.AddEnemyType("Characters/enemyMedium.json", "myFleshLobber");
	
	characterIHub.AddItemType("Items/sword.json");
	characterIHub.AddItemType("Items/shield.json");
	characterIHub.AddItemType("Items/potion.json");
}

CharacterManager::~CharacterManager()
{

}

void CharacterManager::Update(int p_target, float p_deltaT)
{
	if (clickedOnEnemy)
	{
		for (auto it = characters.begin(); it != characters.end(); it++)
		{
			if (it->GetName().compare(characterMoving) == 0)
			{
				if (it->isAttacking() == true)
				{
					clickedOnEnemy = false;
					//characterIHub.DamageEnemy(targetedEnemy, characterMoving);
					for (int i = 0; i < enemies.size(); i++)
					{
						if (enemies.at(i).GetName().compare(targetedEnemy) == 0)
						{
							enemies.at(i).TakeDamage(it->GetName());
						}
					}
				}
			}
		}
	}

	if (enemiesAttacking)
	{
		std::map<std::string, std::string>::iterator it;
		std::vector<std::string> deletions;
		for (it = enemyAttacks.begin(); it != enemyAttacks.end(); it++)
		{
			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies.at(i).isAttacking() == true && enemies.at(i).GetName().compare(it->first) == 0)
				{
					//characterIHub.DamageCharacter(it->second, it->first);
					deletions.push_back(it->first);
					for (auto itc = characters.begin(); itc != characters.end(); itc++)
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
			enemyAttacks.erase(deletions.at(i));
		if(enemyAttacks.size() == 0)
			enemiesAttacking = false;
	}
  
	// TODO - remove this
	static int startpos[] = { 100, 100, 180 };
	static double animtime[] = { 0.0, 0.0, 0.0 };
	// ====

	for (int i = 0; i < characters.size(); i++)
	{
		if (i < characters.size())
		{
			if (characters.at(i).InitDamage())
			{
				characterIHub.DamageCharacter(characters.at(i).GetName(), characters.at(i).GetAttacker());
			}

			if (characterIHub.GetStat(characters.at(i).GetName(), "HP") <= 0.0f)
			{
				characters.at(i).InitDeath();
				if (characters.at(i).GetDeathTimer() >= 99.0f)
				{
					characters.erase(characters.begin() + i);
				}
			}
		}
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		if (i < enemies.size())
		{
			if (enemies.at(i).InitDamage())
			{
				characterIHub.DamageEnemy(enemies.at(i).GetName(), enemies.at(i).GetAttacker());
			}

			if (characterIHub.GetStat(enemies.at(i).GetName(), "HP") <= 0.0f)
			{
				enemies.at(i).InitDeath();
				if (enemies.at(i).GetDeathTimer() >= 99.0f)
				{
					if (items.size() < m_itemCap)
						SpawnItem(enemies.at(i).GetTile());

					enemies.erase(enemies.begin() + i);
					
					if (m_scoreTracker != nullptr)
						m_scoreTracker->AddScore(1);
				}
			}
		}
	}

	timeBetween += p_deltaT;
	currTarget = p_target;

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i].Update(p_deltaT);

		float healthPercent = characterIHub.GetStat(enemies[i].GetName(), "HP") / characterIHub.GetStat(enemies[i].GetName(), "Health");
		enemies[i].SetHealthbarPercent(healthPercent);
	}

	for (int i = 0; i < items.size(); i++)
		items[i]->Update(p_deltaT);

	m_hud->GetElement("Unit_1_Indicator")->SetVisible(false);
	m_hud->GetElement("Unit_2_Indicator")->SetVisible(false);
	m_hud->GetElement("Unit_3_Indicator")->SetVisible(false);

	//Update Heroes and check for target
	int characterCount = 0;
	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		it->Update(p_deltaT);

		// check if unit is hovered over for hud indicator
		if (p_target == it->GetTile()) {
			if (it->GetName() == "Player1") {
				m_hud->GetElement("Unit_1_Indicator")->SetVisible(true);
			}
			else if (it->GetName() == "Player2") {
				m_hud->GetElement("Unit_2_Indicator")->SetVisible(true);
			}
			else if (it->GetName() == "Player3") {
				m_hud->GetElement("Unit_3_Indicator")->SetVisible(true);
			}
		}

		// check for items
		for (int i = 0; i < items.size(); i++) {
			if (glm::length(it->GetPos() - items[i]->GetPos()) < 0.25) {

				characterIHub.GivePlayerItem(it->GetName(), items[i]->GetName());

				delete items[i];
				items.erase(items.begin() + i);
				i--;
			}
		}

		// apply health to hud
		if (characterCount < 3) {
			float health, maxhealth;
			health = characterIHub.GetStat(characters[characterCount].GetName(), "HP");
			maxhealth = characterIHub.GetStat(characters[characterCount].GetName(), "Health");
			m_hud->SetVar("UnitHealth" + std::to_string(characterCount + 1), std::to_string((int)std::ceil(health)));
			m_hud->SetVar("UnitHealthMax" + std::to_string(characterCount + 1), std::to_string((int)std::ceil(maxhealth)));
			m_hud->GetElement("healthbar_unit_" + std::to_string(characterCount + 1))->SetW(314.0 * health / maxhealth);
			if (startpos[characterCount] != health) {
				animtime[characterCount] += p_deltaT * 1.5;

				m_hud->GetElement("healthbar_unit_" + std::to_string(characterCount + 1) + "_highlight")->SetW(314.0 * wolf::Math::lerp(startpos[characterCount], health, wolf::Math::easeIn(std::min(1.0, animtime[characterCount]))) / maxhealth);
				if (animtime[characterCount] >= 1.0) {
					startpos[characterCount] = health;
					animtime[characterCount] = 0.0;
				}
			}
		}

		characterCount++;
	}

	//Check if mouse pressed on top of hero
	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == false && timeBetween >= 0.2f)
	{		
		prevTarget = currTarget;
		for (auto it = characters.begin(); it != characters.end(); it++)
		{
			if (it->GetTile() == currTarget && !it->getHasMoved())
			{
				targetName = it->GetName();
				targeting = true;
				timeBetween = 0.0f;
				it->setSelected(true);
				it = characters.end();
				it--;
			}
			else
			{
				targeting = false;
				timeBetween = 1.0f;
			}
		}
	}

	//check if mouse pressed for second time away from hero
	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == true && timeBetween >= 0.2f)
	{
		bool heroPositionedOnTile = false;
		for (int i = 0; i < characters.size(); i++)
		{
			for (auto it = characters.begin(); it != characters.end(); it++)
			{
				if (it->GetTile() == currTarget)
					heroPositionedOnTile = true;
			}
		}
		//Check if not clicking on hero position or double clicking on hero
		if (!heroPositionedOnTile)
		{
			bool targetingEnemy = false;
			std::string targetEnemy = "";
			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies.at(i).GetTile() == currTarget)
				{
					targetEnemy = enemies.at(i).GetName();
					targetingEnemy = true;
				}
			}
			//Check if targeting an Enemy
			if (targetingEnemy)
			{
				targeting = false;
				timeBetween = 0.0f;
				for (auto it = characters.begin(); it != characters.end(); it++)
				{
					if (it->GetName().compare(targetName) == 0)
					{
						it->setSelected(false);

						std::vector<int> path = grid->GetPathway(prevTarget, currTarget);
						if (path.size() > 0)
						{
							it->Move(path, movementTime, true);
							it->SetTile(path.at(path.size() - 2));
							clickedOnEnemy = true;
							characterMoving = it->GetName();
							targetedEnemy = targetEnemy;
							it = characters.end();
							it--;
						}
					}
				}
			}
			else
			//If not targeting enemy
			{
				targeting = false;
				timeBetween = 0.0f;
				for (auto it = characters.begin(); it != characters.end(); it++)
				{
					if (it->GetName().compare(targetName) == 0)
					{
						it->setSelected(false);

						std::vector<int> path = grid->GetPathway(prevTarget, currTarget);
						if (path.size() > 0)
						{
							it->Move(path, movementTime, false);
							it->SetTile(path.at(path.size() - 1));
							it = characters.end();
							it--;
						}
					}
				}
			}
		}
	}
}

void CharacterManager::Render(glm::mat4 p_view, glm::mat4 p_proj, wolf::RenderFilterType type)
{
	for (auto it = characters.begin(); it != characters.end(); it++)
		it->Render(p_view, p_proj, type);
	
	for (int i = 0; i < enemies.size(); i++)
		enemies.at(i).Render(p_view, p_proj, type);

	for (int i = 0; i < items.size(); i++)
		items[i]->Render(p_view, p_proj, type);
}

void CharacterManager::MoveEnemies(int length)
{
	bool attacking = false;
	for (int i = 0; i < enemies.size(); i++)
	{
		std::vector<int> pathToHero = PathTowardsClosestHero(i, length);
		for (auto it = characters.begin(); it != characters.end(); it++)
		{
			for (int j = 0; j < pathToHero.size(); j++)
			{
				if (pathToHero.at(j) == it->GetTile())
				{
					attacking = true;
					enemyAttacks.insert(std::pair<std::string, std::string>(enemies.at(i).GetName(), it->GetName()));
					enemiesAttacking = true;
				}
			}

		}

		if (attacking)
		{
			enemies.at(i).Move(pathToHero, movementTime, true);
			enemies.at(i).SetTile(pathToHero.at(pathToHero.size() - 2));
		}
		else
		{
			enemies.at(i).Move(pathToHero, movementTime, false);
			enemies.at(i).SetTile(pathToHero.at(pathToHero.size() - 1));
		}

		attacking = false;
	}
}

void CharacterManager::SpawnEnemy(int pos)
{
	m_enemyCount++;
	if (m_enemyCount > m_enemyCap * m_enemyCap * 0.5)
		m_enemyCap++;

	int unitType = wolf::RNG::GetRandom(0, 1);

	CharacterUnits Enemy((unitType)?"units/myskeleton.bmw":"units/myfleshlobber.bmw", "animatable_untextured", pos, ((unitType)?"mySkeleton":"myFleshLobber")+std::to_string(m_enemyCount), grid, (unitType)?0.03:0.07, false, glm::vec3(0.7, 0.1, 0));
	characterIHub.AddEnemyType((unitType)?"Characters/enemyLight.json":"Characters/enemyMedium.json", Enemy.GetName());

	enemies.push_back(Enemy);
}

void CharacterManager::SpawnEnemies()
{
	while (enemies.size() < m_enemyCap) {
		int pos = grid->GetRandomBorder();
		while (grid->isMountain(pos) || IsCharOnTile(pos))
			pos = grid->GetRandomBorder();

		SpawnEnemy(pos);
	}
}

void CharacterManager::SpawnItem(int pos)
{
	int itemType = wolf::RNG::GetRandom(1, 3);

	if (itemType == 1)
		items.push_back(new Item("potion.bmw", "unlit_texture", pos, "Items/potion.json", "Potion", grid));
	else if (itemType == 2)
		items.push_back(new Item("sword1.bmw", "unlit_texture", pos, "Items/sword.json", "Sword", grid));
	else
		items.push_back(new Item("shield.bmw", "unlit_texture", pos, "Items/shield.json", "Shield", grid));
}

std::vector<int> CharacterManager::PathTowardsClosestHero(int enemyIndex, int length)
{
	std::vector<int> path;
	std::vector<int> endPath;
	int closest = -1;
	int distance = 10000;
	int current = 0;

	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		path = grid->GetPathway(enemies.at(enemyIndex).GetTile(), it->GetTile());
		if (path.size() < distance)
		{
			distance = path.size();
			closest = current;
			endPath = path;
		}
		current++;
	}
	if(endPath.size() > length + 1)
		endPath.resize(length + 1);
	//enemies.at(enemyIndex).SetTile(endPath.at(endPath.size() - 1));
	return endPath;
}

std::string CharacterManager::GetCharacterSelected()
{
	return targetName;
}

std::vector<CharacterUnits>* CharacterManager::getCharacters()
{
	return &characters;
}

std::vector<CharacterUnits>* CharacterManager::getEnemies()
{
	return &enemies;
}

void CharacterManager::SetScoreTracker(ScoreTracker* tracker)
{
	m_scoreTracker = tracker;
}

bool CharacterManager::IsCharOnTile(int pos) {
	for (int i = 0; i < characters.size(); i++)
		if (characters[i].GetTile() == pos)
			return true;

	for (int i = 0; i < enemies.size(); i++)
		if (enemies[i].GetTile() == pos)
			return true;

	return false;
}

void CharacterManager::PreloadCharacterModels()
{
	CharacterUnits("units/mychamp.bmw", "animatable_untextured", 0, "myChamp", grid, 5.0, false, glm::vec3(0.1, 0.8, 0.7));
	CharacterUnits("units/mygiant.bmw", "animatable_untextured", 1, "myGiant", grid, 0.05, false, glm::vec3(0.2, 0.7, 0.3));
	CharacterUnits("units/mylich.bmw", "animatable_untextured", 2, "myLich", grid, 0.03, false, glm::vec3(0.75, 0.65, 0.1));
	CharacterUnits("units/myskeleton.bmw", "animatable_untextured", 3, "mySkeleton", grid, 0.03, false, glm::vec3(0.7, 0.1, 0));
	CharacterUnits("units/myfleshlobber.bmw", "animatable_untextured", 4, "myFleshLobber", grid, 0.03, false, glm::vec3(0.7, 0.1, 0));

	Item("potion.bmw", "unlit_texture", 5, "Items/potion.json", "Potion", grid);
	Item("sword1.bmw", "unlit_texture", 6, "Items/sword.json", "Sword", grid);
	Item("shield.bmw", "unlit_texture", 7, "Items/shield.json", "Shield", grid);
}