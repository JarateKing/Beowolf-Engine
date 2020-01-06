#include "CharacterManager.h"
#include "W_Input.h"

CharacterManager::CharacterManager(HexGrid* p_grid)
{
	CharacterUnits lich("lich/FreeLich.bmw", "animatable", 1, "Lich", p_grid);
	//CharacterUnits footMan("footman/Footman.bmw", "animatable", 2, "FootMan");
	characters.push_back(lich);
	grid = p_grid;
}

CharacterManager::~CharacterManager()
{

}

void CharacterManager::Update(int p_target, float p_deltaT)
{
	timeBetween += p_deltaT;
	currTarget = p_target;

	std::list<CharacterUnits>::iterator it;
	for (it = characters.begin(); it != characters.end(); it++)
	{
		it->Update(p_deltaT);
		if (it->GetTile() == currTarget)
		{
			targetName = it->GetName();
		}
	}

	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == false && timeBetween >= 1.0f)
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

	if (wolf::Input::Instance().isMousePressed(INPUT_LMB) && targeting == true && timeBetween >= 1.0f)
	{
		targeting = false;
		timeBetween = 0.0f;
		for (it = characters.begin(); it != characters.end(); it++)
		{
			if (it->GetName().compare(targetName) == 0)
			{
				it->Move(grid->GetPathway(prevTarget, currTarget), 3.0f);
				it->SetTile(currTarget);
			}
		}
	}
}

void CharacterManager::Render(glm::mat4 p_view, glm::mat4 p_proj, bool p_renderAlphas)
{
	std::list<CharacterUnits>::iterator it;
	for (it = characters.begin(); it != characters.end(); ++it)
	{
		it->Render(p_view, p_proj, p_renderAlphas);
	}
}

