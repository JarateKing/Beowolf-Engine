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
		bool isFirst;
		bool isFirstIter;
		outFile << "{\n";

		isFirst = true;
		outFile << "\"Characters\": [\n";
		for (auto unit : *(m_manager->getCharacters())) {
			if (!isFirst)
				outFile << ",\n";
			isFirst = false;

			outFile << "{\n";
			outFile << "\"Name\": " << unit.GetName() << ",\n";
			outFile << "\"Tile\": " << unit.GetTile() << ",\n";
			outFile << "\"Cooldown\": " << unit.GetCooldown() << ",\n";

			isFirstIter = true;
			outFile << "\"Stats\": {\n";
			for (auto stat : m_hub->GetStats(unit.GetName())) {
				if (!isFirstIter)
					outFile << ",\n";
				isFirstIter = false;

				outFile << stat.first << " = " << stat.second;
			}
			outFile << "\n}\n}";
		}
		outFile << "\n],\n";
		
		isFirst = true;
		outFile << "\"Enemies\": [\n";
		for (auto unit : *(m_manager->getEnemies())) {
			if (!isFirst)
				outFile << ",\n";
			isFirst = false;

			outFile << "{\n";
			outFile << "\"Name\": " << unit.GetName() << ",\n";
			outFile << "\"Tile\": " << unit.GetTile() << ",\n";

			isFirstIter = true;
			outFile << "\"Stats\": {\n";
			for (auto stat : m_hub->GetStats(unit.GetName())) {
				if (!isFirstIter)
					outFile << ",\n";
				isFirstIter = false;

				outFile << stat.first << " = " << stat.second;
			}
			outFile << "\n}\n}";
		}
		outFile << "\n],\n";

		isFirst = true;
		outFile << "\"Items\": [\n";
		for (auto item : *(m_manager->getItems())) {
			if (!isFirst)
				outFile << ",\n";
			isFirst = false;

			outFile << "{\n";
			outFile << "\"Name\": " << item->GetName() << ",\n";
			outFile << "\"Tile\": " << item->GetTile() << "\n";
			outFile << "}";
		}
		outFile << "\n],\n";

		isFirst = true;
		outFile << "\"Grid\": [\n";
		for (int i = 0; i < m_grid->GetSize(); i++) {
			if (!isFirst)
				outFile << ",\n";
			isFirst = false;

			outFile << "{\n";
			outFile << "\"id\": " << i << ",\n";
			outFile << "\"desert\": " << ((m_grid->isDesert(i)) ? "True" : "False") << ",\n";
			outFile << "\"mountain\": " << ((m_grid->isMountain(i)) ? "True" : "False") << ",\n";
			outFile << "\"height\": " << m_grid->GetHeights()[i] << ",\n";
			outFile << "}";
		}
		outFile << "\n]\n";
	}
	outFile.close();
}