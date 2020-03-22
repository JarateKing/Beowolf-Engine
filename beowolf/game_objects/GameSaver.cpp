#include "GameSaver.h"
#include "W_Input.h"
#include "W_Math.h"

GameSaver::GameSaver(wolf::Hud* hud) {
	m_hud = hud;
}

GameSaver::~GameSaver() {}

void GameSaver::Update(float delta) {
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_Z)) {
		m_indicatorTime = 0.0f;

		SaveInfo();
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

void GameSaver::SaveInfo() {
	std::cout << "Character Info:\n";
	for (auto unit : *(m_manager->getCharacters())) {
		std::cout << unit.GetName() << " " << unit.GetTile() << " " << unit.GetCooldown() << "\n";
		for (auto stat : m_hub->GetStats(unit.GetName())) {
			std::cout << stat.first << " = " << stat.second << '\n';
		}
	}
	
	std::cout << "Enemy Info:\n";
	for (auto unit : *(m_manager->getEnemies())) {
		std::cout << unit.GetName() << " " << unit.GetTile() << "\n";
		for (auto stat : m_hub->GetStats(unit.GetName())) {
			std::cout << stat.first << " = " << stat.second << '\n';
		}
	}

	std::cout << "Item Info:\n";
	for (auto item : *(m_manager->getItems())) {
		std::cout << item->GetName() << " " << item->GetTile() << "\n";
	}

	std::cout << "Grid Info:\n";
	for (int i = 0; i < m_grid->GetSize(); i++) {
		std::cout << m_grid->isDesert(i) << " " << m_grid->isMountain(i) << " " << m_grid->GetHeights()[i] << "\n";
	}
}