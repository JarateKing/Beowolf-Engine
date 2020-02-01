#include "StateManager.h"
#include <iostream>
#include <list>
#include "W_HudButton.h"

void StateManager::Update(float delta) {
	if (m_hud != nullptr) {
		if (m_currentState == State::GamestateMainMenu) {
			if (((wolf::HudButton*)m_hud->GetElement("MM_Start_Button"))->IsClicked()) {
				SetState(State::GamestatePlayerTurn);

				m_hud->GetElement("MM_Start_Button")->SetVisible(false);
				m_hud->GetElement("MM_button_bg")->SetVisible(false);
				m_hud->GetElement("MM_Unit_1")->SetVisible(false);
				m_hud->GetElement("MM_Unit_2")->SetVisible(false);
				m_hud->GetElement("MM_Titlecard")->SetVisible(false);
				m_hud->GetElement("MM_Startlabel")->SetVisible(false);
			}
		}
	}
	if (m_charManager != nullptr) {
		if (m_currentState == State::GamestatePlayerTurn) {
			bool hasAllMoved = true;
			
			auto chars = m_charManager->getCharacters();
			for (auto it = chars->begin(); hasAllMoved && it != chars->end(); it++) {
				if (!it->getHasMoved() || it->isMoving() || !it->GetModel()->isAnimDefault() || it->isAttacking())
					hasAllMoved = false;
			}

			if (hasAllMoved)
				SetState(State::GamestateEnemyTurn);
		}
		else if (m_currentState == State::GamestateEnemyTurn) {
			bool hasAllMoved = true;

			auto chars = m_charManager->getEnemies();
			for (auto it = chars->begin(); hasAllMoved && it != chars->end(); it++) {
				if (!it->getHasMoved() || it->isMoving() || !it->GetModel()->isAnimDefault() || it->isAttacking())
					hasAllMoved = false;
			}

			if (hasAllMoved)
				SetState(State::GamestatePlayerTurn);
		}
	}
}

State StateManager::GetState() {
	return m_currentState;
}

void StateManager::SetState(State state) {
	m_currentState = state;

	if (m_charManager != nullptr) {
		if (m_currentState == State::GamestatePlayerTurn) {
			auto chars = m_charManager->getCharacters();
			for (auto it = chars->begin(); it != chars->end(); it++) {
				it->setHasMoved(false);
			}
			auto enemies = m_charManager->getEnemies();
			for (auto it = enemies->begin(); it != enemies->end(); it++) {
				it->setHasMoved(true);
			}

			if (m_hud != nullptr)
				m_hud->SetVar("whoseturn", "Player's");
		}
		else if (m_currentState == State::GamestateEnemyTurn) {
			auto chars = m_charManager->getCharacters();
			for (auto it = chars->begin(); it != chars->end(); it++) {
				it->setHasMoved(true);
			}
			auto enemies = m_charManager->getEnemies();
			for (auto it = enemies->begin(); it != enemies->end(); it++) {
				it->setHasMoved(false);
			}

			if (m_hud != nullptr)
				m_hud->SetVar("whoseturn", "Enemies'");

			m_charManager->MoveEnemies(2);
		}
	}
}

void StateManager::SetCharacterManager(CharacterManager* charMan) {
	if (m_charManager == charMan)
		std::cout << "Note: character manager already tracked by state manager\n";

	m_charManager = charMan;
}

void StateManager::SetHud(wolf::Hud* hud) {
	if (m_hud == hud)
		std::cout << "Note: hud already tracked by state manager\n";

	m_hud = hud;

	if (m_currentState == State::GamestatePlayerTurn)
		m_hud->SetVar("whoseturn", "Player's");
	else if (m_currentState == State::GamestateEnemyTurn)
		m_hud->SetVar("whoseturn", "Enemies'");
}