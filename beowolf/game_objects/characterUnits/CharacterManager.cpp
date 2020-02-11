#include "CharacterManager.h"
#include "W_Input.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"
#include <cmath>
#include "camera/HexSelector.h"

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
	m_cameraTime += p_deltaT;
	m_cameraUnit %= characters.size();
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_F)) {
		if (m_cameraTime < 2.0f) {
			m_cameraUnit = (m_cameraUnit + 1) % characters.size();
		}
		else {
			m_cameraUnit = 0;
		}

		m_cameraTime = 0.0f;
		m_cam->MoveToView(characters[m_cameraUnit].GetPos(), glm::vec3(0, 50 - characters[m_cameraUnit].GetPos().y, -40.0f), 0.35f);
	}

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

	// potentially push up healthbars
	static bool charsAlive[] = { true, true, true };
	if (m_charCount != characters.size()) {
		m_charCount = characters.size();
		bool charsAliveNow[] = { false, false, false };

		for (int i = 0; i < m_charCount; i++) {
			if (characters[i].GetName() == "myChamp")
				charsAliveNow[0] = true;
			if (characters[i].GetName() == "myGiant")
				charsAliveNow[1] = true;
			if (characters[i].GetName() == "myLich")
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

	//Update Heroes and check for target
	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		it->Update(p_deltaT);

		// check for items
		for (int i = 0; i < items.size(); i++) {
			if (glm::length(it->GetPos() - items[i]->GetPos()) < 0.25) {

				characterIHub.GivePlayerItem(it->GetName(), items[i]->GetName());

				delete items[i];
				items.erase(items.begin() + i);
				i--;
			}
		}

		// do hud specific stuff
		if (it->GetName() == "myChamp") {
			if (p_target == it->GetTile())
				m_hud->GetElement("Unit_1_Indicator")->SetVisible(true);

			float health = characterIHub.GetStat(it->GetName(), "HP");
			float maxhealth = characterIHub.GetStat(it->GetName(), "Health");
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
			if (p_target == it->GetTile())
				m_hud->GetElement("Unit_2_Indicator")->SetVisible(true);

			float health = characterIHub.GetStat(it->GetName(), "HP");
			float maxhealth = characterIHub.GetStat(it->GetName(), "Health");
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
			if (p_target == it->GetTile())
				m_hud->GetElement("Unit_3_Indicator")->SetVisible(true);

			float health = characterIHub.GetStat(it->GetName(), "HP");
			float maxhealth = characterIHub.GetStat(it->GetName(), "Health");
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
		//bool withinTarget = false;

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
				for (auto it = characters.begin(); it != characters.end(); it++)
				{
					if (it->GetName().compare(targetName) == 0)
					{
						grid->ClearBlocks();

						std::vector<int> tilesBlocked;

						for (int i = 0; i < enemies.size(); i++)
						{
							if (enemies.at(i).GetName().compare(targetEnemy) != 0)
								tilesBlocked.push_back(enemies.at(i).GetTile());
						}
						for (int i = 0; i < characters.size(); i++)
						{
							if (characters.at(i).GetName().compare(targetName) != 0)
								tilesBlocked.push_back(characters.at(i).GetTile());
						}
						BlockTiles(tilesBlocked);

						std::vector<int> path = grid->GetPathway(prevTarget, currTarget);

						int pSize = path.size();
						pSize--;
						int mMove = (int)characterIHub.GetStat(it->GetName(), "MaxMovement");

						if ((path.size() > 0) && (pSize <= mMove))
						{
							if (pSize <= mMove)
							{
								targeting = false;
								timeBetween = 0.0f;
								it->setSelected(false);
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
			}
			else
			//If not targeting enemy
			{
				for (auto it = characters.begin(); it != characters.end(); it++)
				{
					if (it->GetName().compare(targetName) == 0)
					{
						grid->ClearBlocks();

						std::vector<int> tilesBlocked;

						for (int i = 0; i < enemies.size(); i++)
						{
							tilesBlocked.push_back(enemies.at(i).GetTile());
						}
						for (int i = 0; i < characters.size(); i++)
						{
							if(characters.at(i).GetName().compare(targetName) != 0)
								tilesBlocked.push_back(characters.at(i).GetTile());
						}
						BlockTiles(tilesBlocked);

						std::vector<int> path = grid->GetPathway(prevTarget, currTarget);

						int pSize = path.size();
						pSize--;
						int mMove = (int)characterIHub.GetStat(it->GetName(), "MaxMovement");

						if ((path.size() > 0) && (pSize <= mMove))
						{
							targeting = false;
							timeBetween = 0.0f;
							it->setSelected(false);
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

void CharacterManager::MoveEnemies()
{
	bool attacking = false;
	for (int i = 0; i < enemies.size(); i++)
	{
		int enemyMovement = (int)characterIHub.GetStat(enemies.at(i).GetName(), "MaxMovement");
		std::vector<int> pathToHero = PathTowardsClosestHero(i, enemyMovement);
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


		if (pathToHero.size() > 0)
		{
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
		}

		attacking = false;
	}
}

void CharacterManager::SpawnEnemy(int pos, float multiplier)
{
	m_enemiesSpawnedTotal++;

	m_enemyCount++;
	if (m_enemyCount > m_enemyCap * m_enemyCap * 0.5)
		m_enemyCap++;

	int unitType = wolf::RNG::GetRandom(0, 1);

	CharacterUnits Enemy((unitType)?"units/myskeleton.bmw":"units/myfleshlobber.bmw", "animatable_untextured", pos, ((unitType)?"mySkeleton":"myFleshLobber")+std::to_string(m_enemyCount), grid, (unitType)?0.03:0.07, false, glm::vec3(0.7, 0.1, 0));
	characterIHub.AddEnemyType((unitType)?"Characters/enemyLight.json":"Characters/enemyMedium.json", Enemy.GetName());

	characterIHub.UpdateStat(Enemy.GetName(), "HP", characterIHub.GetStat(Enemy.GetName(), "HP") * multiplier);
	characterIHub.UpdateStat(Enemy.GetName(), "Health", characterIHub.GetStat(Enemy.GetName(), "Health") * multiplier);
	characterIHub.UpdateStat(Enemy.GetName(), "MaxAttack", characterIHub.GetStat(Enemy.GetName(), "MaxAttack") * multiplier);
	characterIHub.UpdateStat(Enemy.GetName(), "Defense", characterIHub.GetStat(Enemy.GetName(), "Defense") * multiplier);

	enemies.push_back(Enemy);
}

void CharacterManager::SpawnEnemies()
{
	while (enemies.size() < m_enemyCap) {
		int iters = 0;
		int pos = grid->GetRandomBorder();
		while (iters++ < 100 && (grid->isMountain(pos) || IsCharOnTile(pos)))
			pos = grid->GetRandomBorder();

		if (iters >= 100)
			return;
		
		float multiply = m_enemiesSpawnedTotal / 100.0f + 1.0f;

		SpawnEnemy(pos, multiply);
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

	grid->ClearBlocks();
	std::vector<int> tilesBlocked;

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies.at(i).GetName().compare(enemies.at(enemyIndex).GetName()) != 0)
			tilesBlocked.push_back(enemies.at(i).GetTile());
	}

	BlockTiles(tilesBlocked);

	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		path = grid->GetPathway(enemies.at(enemyIndex).GetTile(), it->GetTile());
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

void CharacterManager::BlockEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		blocked.push_back(glm::vec3(enemies.at(i).GetPos().x, 0.0f, enemies.at(i).GetPos().z));
		grid->BlockNodePositions(glm::vec3(enemies.at(i).GetPos().x, 0.0f, enemies.at(i).GetPos().z));
	}
}

void CharacterManager::BlockCharacters()
{
	for (int i = 0; i < characters.size(); i++)
	{
		blocked.push_back(glm::vec3(characters.at(i).GetPos().x, 0.0f, characters.at(i).GetPos().z));
		grid->BlockNodePositions(glm::vec3(characters.at(i).GetPos().x, 0.0f, characters.at(i).GetPos().z));
	}
}

void CharacterManager::BlockTiles(std::vector<int> tiles)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		grid->BlockNodePositions(glm::vec3(grid->GetPos().at(tiles.at(i)).x, 0.0f, grid->GetPos().at(tiles.at(i)).y));
	}
}
void CharacterManager::SetScoreTracker(ScoreTracker* tracker)
{
	m_scoreTracker = tracker;
}

void CharacterManager::SetCamera(Camera* cam)
{
	m_cam = cam;
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
