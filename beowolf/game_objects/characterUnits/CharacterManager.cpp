#include "CharacterManager.h"
#include "W_Input.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"

CharacterManager::CharacterManager(HexGrid* p_grid)
{
	CharacterUnits player1("units/mychamp.bmw", "animatable_untextured", 2, "myChamp", p_grid, 5.0, false, glm::vec3(0.1, 0.8, 0.7));
	characterIHub.AddCharacter("Characters/hero1.json", "myChamp");
	CharacterUnits player2("units/mygiant.bmw", "animatable_untextured", 3, "myGiant", p_grid, 0.05, false, glm::vec3(0.1, 0.8, 0.7));
	characterIHub.AddCharacter("Characters/hero2.json", "myGiant");
	CharacterUnits player3("units/mylich.bmw", "animatable_untextured", 4, "myLich", p_grid, 0.03, false, glm::vec3(0.1, 0.8, 0.7));
	characterIHub.AddCharacter("Characters/hero3.json", "myLich");

	characters.push_back(player1);
	characters.push_back(player2);
	characters.push_back(player3);

	CharacterUnits Enemy1("units/myskeleton.bmw", "animatable_untextured", 110, "mySkeleton", p_grid, 0.03, false, glm::vec3(0.7, 0.1, 0));
	characterIHub.AddEnemyType("Characters/enemyLight.json", "mySkeleton");
	CharacterUnits Enemy2("units/myfleshlobber.bmw", "animatable_untextured", 120, "myFleshLobber", p_grid, 0.06, false, glm::vec3(0.7, 0.1, 0));
	characterIHub.AddEnemyType("Characters/enemyMedium.json", "myFleshLobber");

	enemies.push_back(Enemy1);
	enemies.push_back(Enemy2);

	grid = p_grid;
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
					characterIHub.DamageEnemy(targetedEnemy, characterMoving);
					for (int i = 0; i < enemies.size(); i++)
					{
						if (enemies.at(i).GetName().compare(targetedEnemy) == 0)
						{
							std::cout << it->GetName() << " is attacking" << std::endl;
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
					characterIHub.DamageCharacter(it->second, it->first);
					deletions.push_back(it->first);
					for (auto itc = characters.begin(); itc != characters.end(); itc++)
					{
						if (itc->GetName().compare(it->second) == 0)
						{
							std::cout << it->second << " is attacking" << std::endl;
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
	timeBetween += p_deltaT;
	currTarget = p_target;

	for (int i = 0; i < enemies.size(); i++)
		enemies[i].Update(p_deltaT);

	for (int i = 0; i < items.size(); i++)
		items[i]->Update(p_deltaT);

	//Update Heroes and check for target
	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		it->Update(p_deltaT);

		// check for items
		for (int i = 0; i < items.size(); i++) {
			if (glm::length(it->GetPos() - items[i]->GetPos()) < 0.25) {
				auto statmap = items[i]->GetStats();
				for (auto jt = statmap.begin(); jt != statmap.end(); jt++)
					it->ModifyStats(jt->first, jt->second);

				delete items[i];
				items.erase(items.begin() + i);
				i--;
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

void CharacterManager::SpawnEnemies(int numSpawn, std::string enemyFile)
{

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

std::list<CharacterUnits>* CharacterManager::getCharacters()
{
	return &characters;
}

std::vector<CharacterUnits>* CharacterManager::getEnemies()
{
	return &enemies;
}
