#include "StateManager.h"
#include <iostream>
#include <list>

void StateManager::Update(float delta) {
	if (m_charManager != nullptr) {
		if (m_currentState == State::GamestatePlayerTurn) {
			bool hasAllMoved = true;
			
			std::list<CharacterUnits> chars = m_charManager->getCharacters();
			for (auto it = chars.begin(); hasAllMoved && it != chars.end(); it++) {
				if (!it->getHasMoved() || it->isMoving())
					hasAllMoved = false;
			}

			if (hasAllMoved)
				SetState(State::GamestateEnemyTurn);
		}
		else if (m_currentState == State::GamestateEnemyTurn) {
			bool hasAllMoved = true;

			for (auto it = m_charManager->getEnemies().begin(); hasAllMoved && it != m_charManager->getEnemies().end(); it++) {
				if (!it->getHasMoved() || it->isMoving())
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
			std::list<CharacterUnits> chars = m_charManager->getCharacters();
			for (auto it = chars.begin(); it != chars.end(); it++) {
				it->setHasMoved(false);
			}
			for (auto it = m_charManager->getEnemies().begin(); it != m_charManager->getEnemies().end(); it++) {
				it->setHasMoved(true);
			}
		}
		else if (m_currentState == State::GamestateEnemyTurn) {
			std::list<CharacterUnits> chars = m_charManager->getCharacters();
			for (auto it = chars.begin(); it != chars.end(); it++) {
				it->setHasMoved(true);
			}
			for (auto it = m_charManager->getEnemies().begin(); it != m_charManager->getEnemies().end(); it++) {
				it->setHasMoved(false);
			}

			m_charManager->MoveEnemies(2);
		}
	}
}

void StateManager::SetCharacterManager(CharacterManager* charMan) {
	if (m_charManager == charMan)
		std::cout << "Note: character manager already tracked by state manager\n";

	m_charManager = charMan;
}