#include "CharacterManager.h"
#include "W_Input.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"
#include <cmath>

CharacterManager::CharacterManager(HexGrid* p_grid, wolf::Hud* p_hud)
{
	CharacterUnits player1("units/mychamp.bmw", "animatable_untextured", 2, "Player1", p_grid, 5.0, false, glm::vec3(0.1, 0.8, 0.7));
	CharacterUnits player2("units/mygiant.bmw", "animatable_untextured", 3, "Player2", p_grid, 0.05, false, glm::vec3(0.1, 0.8, 0.7));
	CharacterUnits player3("units/mylich.bmw", "animatable_untextured", 4, "Player3", p_grid, 0.03, false, glm::vec3(0.1, 0.8, 0.7));
	characters.push_back(player1);
	characters.push_back(player2);
	characters.push_back(player3);

	CharacterUnits knight("units/myskeleton.bmw", "animatable_untextured", 110, "Knight11", p_grid, 0.03, false, glm::vec3(0.7, 0.1, 0));
	CharacterUnits knight2("units/myfleshlobber.bmw", "animatable_untextured", 120, "Knight12", p_grid, 0.06, false, glm::vec3(0.7, 0.1, 0));
	enemies.push_back(knight);
	enemies.push_back(knight2);

	grid = p_grid;
	m_hud = p_hud;
}

CharacterManager::~CharacterManager()
{

}

void CharacterManager::Update(int p_target, float p_deltaT)
{
	// TODO - remove this
	static int health[] = { 100, 100, 180 };
	static int maxhealth[] = { 100, 100, 180 };
	static int startpos[] = { 100, 100, 180 };
	static double animtime[] = { 0.0, 0.0, 0.0 };

	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_1)) {
		health[0] = std::max(0, health[0] - 10);
	}
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_2)) {
		health[1] = std::max(0, health[1] - 10);
	}
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_3)) {
		health[2] = std::max(0, health[2] - 10);
	}
	// ====

	timeBetween += p_deltaT;
	currTarget = p_target;

	for (int i = 0; i < enemies.size(); i++)
		enemies[i].Update(p_deltaT);

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
				auto statmap = items[i]->GetStats();
				for (auto jt = statmap.begin(); jt != statmap.end(); jt++)
					it->ModifyStats(jt->first, jt->second);

				delete items[i];
				items.erase(items.begin() + i);
				i--;
			}
		}

		// apply health to hud
		if (characterCount < 3) {
			m_hud->SetVar("UnitHealth" + std::to_string(characterCount + 1), std::to_string(health[characterCount]));
			m_hud->SetVar("UnitHealthMax" + std::to_string(characterCount + 1), std::to_string(maxhealth[characterCount]));
			m_hud->GetElement("healthbar_unit_" + std::to_string(characterCount + 1))->SetW(314.0 * health[characterCount] / maxhealth[characterCount]);
			if (startpos[characterCount] != health[characterCount]) {
				animtime[characterCount] += p_deltaT * 1.5;

				m_hud->GetElement("healthbar_unit_" + std::to_string(characterCount + 1) + "_highlight")->SetW(314.0 * wolf::Math::lerp((double)startpos[characterCount], (double)health[characterCount], wolf::Math::easeIn(std::min(1.0, animtime[characterCount]))) / maxhealth[characterCount]);
				if (animtime[characterCount] >= 1.0) {
					startpos[characterCount] = health[characterCount];
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
		targeting = false;
		timeBetween = 0.0f;
		for (auto it = characters.begin(); it != characters.end(); it++)
		{
			if (it->GetName().compare(targetName) == 0)
			{
				it->setSelected(false);

				std::vector<int> path = grid->GetPathway(prevTarget, currTarget);
				if (path.size() > 0) {
					it->Move(path, movementTime);
					it->SetTile(currTarget);
					it = characters.end();
					it--;
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
	endPath.resize(length + 1);
	enemies.at(enemyIndex).SetTile(endPath.at(endPath.size() - 1));
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
