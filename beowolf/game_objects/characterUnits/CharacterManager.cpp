#include "CharacterManager.h"
#include "W_Input.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"

CharacterManager::CharacterManager(HexGrid* p_grid)
{
	CharacterUnits lich("lich/FreeLich.bmw", "animatable", 1, "Lich", p_grid);
	CharacterUnits lich2("lich/FreeLich.bmw", "animatable", 1, "Lich", p_grid);
	characters.push_back(lich);
	characters.push_back(lich2);
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
	for (auto it = enemies.begin(); it != enemies.end(); it++)
	{
		it->Update(p_deltaT);
	}

	//Update Heroes and check for target
	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		it->Update(p_deltaT);
		if (it->GetTile() == currTarget)
		{
			targetName = it->GetName();
		}
	}

	//Check if mouse pressed on top of hero
	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == false && timeBetween >= 0.2f)
	{
		targeting = true;
		timeBetween = 0.0f;
		prevTarget = currTarget;
		for (auto it = characters.begin(); it != characters.end(); it++)
		{
			it->Update(p_deltaT);
			if (it->GetTile() == currTarget)
			{
				targetName = it->GetName();
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
			}
		}
	}
}

void CharacterManager::Render(glm::mat4 p_view, glm::mat4 p_proj, bool p_renderAlphas)
{
	for (auto it = characters.begin(); it != characters.end(); it++)
	{
		it->Render(p_view, p_proj, p_renderAlphas);
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i).Render(p_view, p_proj, p_renderAlphas);
	}

	for (int i = 0; i < items.size(); i++)
		items[i].Render(p_view, p_proj, p_renderAlphas);
}

void CharacterManager::MoveEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		
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

std::vector<int> CharacterManager::PathTowardsClosestHero(int tile)
{
	return std::vector<int>();
}

std::string CharacterManager::GetCharacterSelected()
{
	return targetName;
}
