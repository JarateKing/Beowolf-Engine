#include "CharacterManager.h"
#include "W_Input.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"

CharacterManager::CharacterManager(HexGrid* p_grid)
{
	CharacterUnits lich("Knights/RedKnight.bmw", "animatable", 1, "Knight1", p_grid, 0.03, true);
	CharacterUnits lich2("Knights/BlueKnight.bmw", "animatable", 2, "Knight2", p_grid, 0.03, true);
	characters.push_back(lich);
	characters.push_back(lich2);

	CharacterUnits knight("Knights/RedKnight.bmw", "animatable", 10, "Knight11", p_grid, 0.03, true);
	CharacterUnits knight2("Knights/BlueKnight.bmw", "animatable", 20, "Knight12", p_grid, 0.03, true);
	enemies.push_back(knight);
	enemies.push_back(knight2);

	grid = p_grid;
}

CharacterManager::~CharacterManager()
{

}

void CharacterManager::Update(float p_deltaT)
{
	for (int i = 0; i < items.size(); i++)
		items[i].Update(p_deltaT);
}

void CharacterManager::Update(int p_target, float p_deltaT)
{
	timeBetween += p_deltaT;
	currTarget = p_target;

	//Update Enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i).Update(p_deltaT);
	}

	std::list<CharacterUnits>::iterator it;
	//Update Heroes and check for target
	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		it->Update(p_deltaT);

		// check for items
		for (int i = 0; i < items.size(); i++) {
			if (glm::length(it->GetPos() - items[i].GetPos()) < 0.25) {
				std::cout << 'y';
			}
		}
	}

	//Check if mouse pressed on top of hero
	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == false && timeBetween >= 0.2f)
	{		
		prevTarget = currTarget;
		for (auto it = characters.begin(); it != characters.end(); it++)
		{
			if (it->GetTile() == currTarget)
			{
				targetName = it->GetName();
				targeting = true;
				timeBetween = 0.0f;
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
		targeting = false;
		timeBetween = 0.0f;
		for (auto it = characters.begin(); it != characters.end(); it++)
		{
			if (it->GetName().compare(targetName) == 0)
			{
				it->Move(grid->GetPathway(prevTarget, currTarget), movementTime);
				it->SetTile(currTarget);
				it = characters.end();
				it--;
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
		items[i].Render(p_view, p_proj, type);
}

void CharacterManager::MoveEnemies(int length)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i).Move(PathTowardsClosestHero(i, length), movementTime);
	}
}

void CharacterManager::SpawnEnemies(int numSpawn, std::string enemyFile)
{

}

void CharacterManager::SpawnItem(int pos)
{
	int itemType = wolf::RNG::GetRandom(1, 3);

	if (itemType == 1)
		items.push_back(Item("potion.bmw", "unlit_texture", pos, "Items/potion.json", "Potion", grid));
	else if (itemType == 2)
		items.push_back(Item("sword1.bmw", "unlit_texture", pos, "Items/sword.json", "Sword", grid));
	else
		items.push_back(Item("shield.bmw", "unlit_texture", pos, "Items/shield.json", "Shield", grid));
}

std::vector<int> CharacterManager::PathTowardsClosestHero(int enemyIndex, int length)
{
	std::vector<int> path;
	std::vector<int> endPath;
	int closest = -1;
	int distance = 10000;
	int current = 0;
	std::list<CharacterUnits>::iterator it;
	for (it = characters.begin(); it != characters.end(); it++)
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
	endPath.resize(length + 1);
	enemies.at(enemyIndex).SetTile(endPath.at(endPath.size() - 1));
	return endPath;
}

std::string CharacterManager::GetCharacterSelected()
{
	return targetName;
}
