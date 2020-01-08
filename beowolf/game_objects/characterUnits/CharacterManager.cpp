#include "CharacterManager.h"
#include "W_Input.h"

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

void CharacterManager::Update(int p_target, float p_deltaT)
{
	timeBetween += p_deltaT;
	currTarget = p_target;

	//Update Enemies
	for (it = enemies.begin(); it != enemies.end(); it++)
	{
		it->Update(p_deltaT);
	}

	//Update Heroes and check for target
	for (it = characters.begin(); it != characters.end(); it++)
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
		for (it = characters.begin(); it != characters.end(); it++)
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
		for (it = characters.begin(); it != characters.end(); it++)
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
	for (it = characters.begin(); it != characters.end(); it++)
	{
		it->Render(p_view, p_proj, p_renderAlphas);
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i).Render(p_view, p_proj, p_renderAlphas);
	}
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

void CharacterManager::MoveTowardsClosestHero(CharacterUnits enemy)
{

}

std::string CharacterManager::GetCharacterSelected()
{
	return targetName;
}
