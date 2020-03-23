#include "GameSaver.h"
#include "W_Input.h"
#include "W_Math.h"
#include <fstream>

GameSaver::GameSaver(wolf::Hud* hud) {
	m_hud = hud;
}

GameSaver::~GameSaver() {}

void GameSaver::Update(float delta) {
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_Z)) {
		m_indicatorTime = 0.0f;

		SaveInfo("savefile.json");
	}

	m_indicatorTime += delta;
	if (m_indicatorTime >= 1.0f)
		m_indicatorTime = 1.0f;

	for (auto element : m_hud->GetElementsByTag("save_indicator"))
		element->SetAlpha(wolf::Math::lerp(1.0f, 0.0f, wolf::Math::ease(m_indicatorTime)));
}

void GameSaver::SetInfo(CharacterManager* manager, ScoreTracker* tracker, HexGrid* grid) {
	m_manager = manager;
	m_tracker = tracker;
	m_grid = grid;

	m_hub = m_manager->GetCharacterHub();
}

void GameSaver::SaveInfo(std::string filename) {
	std::ofstream outFile;
	outFile.open(filename);
	if (outFile.is_open()) {
		outFile << "Character Info:\n";
		for (auto unit : *(m_manager->getCharacters())) {
			outFile << unit.GetName() << " " << unit.GetTile() << " " << unit.GetCooldown() << "\n";
			for (auto stat : m_hub->GetStats(unit.GetName())) {
				outFile << stat.first << " = " << stat.second << '\n';
			}
		}
		
		outFile << "Enemy Info:\n";
		for (auto unit : *(m_manager->getEnemies())) {
			outFile << unit.GetName() << " " << unit.GetTile() << "\n";
			for (auto stat : m_hub->GetStats(unit.GetName())) {
				outFile << stat.first << " = " << stat.second << '\n';
			}
		}

		outFile << "Item Info:\n";
		for (auto item : *(m_manager->getItems())) {
			outFile << item->GetName() << " " << item->GetTile() << "\n";
		}

		outFile << "Grid Info:\n";
		for (int i = 0; i < m_grid->GetSize(); i++) {
			outFile << m_grid->isDesert(i) << " " << m_grid->isMountain(i) << " " << m_grid->GetHeights()[i] << "\n";
		}
	}
	outFile.close();
}