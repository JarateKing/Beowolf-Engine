#include "StateManager.h"
#include <iostream>
#include <list>
#include "W_HudButton.h"
#include "W_Math.h"

void StateManager::Update(float delta) {
	static float time = 0;
	static bool movingcamera = false;
	if (movingcamera) {
		time += delta;

		if (time >= 1.5) {
			time = 1.5;
			movingcamera = false;
		}

		m_cam->SetVerticleAngle(wolf::Math::lerp(0.5, -0.7831, wolf::Math::easeOut(time / 1.5)));
		m_cam->ForceAngleUpdate();

		if (m_hud != nullptr)
			for (auto element : m_hud->GetElementsByTag("ingame"))
				element->SetAlpha(wolf::Math::easeOut(time / 1.5));
	}

	if (m_hud != nullptr) {
		if (m_currentState == State::GamestateMainMenu) {
			if (((wolf::HudButton*)m_hud->GetElement("MM_Start_Button"))->IsClicked()) {
				SetState(State::GamestatePlayerTurn);

				for (auto element : m_hud->GetElementsByTag("mainmenu"))
					element->SetVisible(false);

				for (auto element : m_hud->GetElementsByTag("ingame")) {
					element->SetVisible(true);
					element->SetAlpha(0);
				}

				if (m_cam != nullptr)
					movingcamera = true;
			}
		}
	}

	if (m_charManager != nullptr) {
		if (m_currentState == State::GamestatePlayerTurn) {
			bool hasAllMoved = true;
			
			auto chars = m_charManager->getCharacters();
			for (auto it = chars->begin(); hasAllMoved && it != chars->end(); it++) {
				if (!it->getHasMoved() || it->isMoving() || it->isDying() || it->isAttacking())
					hasAllMoved = false;
				
			}
			auto enemies = m_charManager->getEnemies();
			for (auto it = enemies->begin(); hasAllMoved && it != enemies->end(); it++) {
				if (it->isDying())
					hasAllMoved = false;
			}

			if (hasAllMoved)
				SetState(State::GamestateEnemyTurn);
		}
		else if (m_currentState == State::GamestateEnemyTurn) {
			bool hasAllMoved = true;

			auto chars = m_charManager->getEnemies();
			for (auto it = chars->begin(); hasAllMoved && it != chars->end(); it++) {
				if (!it->getHasMoved() || it->isMoving() || it->isDying() || it->isAttacking())
					hasAllMoved = false;
			}
			auto enemies = m_charManager->getCharacters();
			for (auto it = enemies->begin(); hasAllMoved && it != enemies->end(); it++) {
				if (it->isDying() && !it->GetDeathTimer() != 100.0f)
					hasAllMoved = false;
			}

			if (hasAllMoved) {
				if (enemies->size() == 0)
					SetState(State::GamestatePlayerLost);
				else
					SetState(State::GamestatePlayerTurn);
			}
		}
	}
}

State StateManager::GetState() {
	return m_currentState;
}

void StateManager::SetState(State state) {
	m_currentState = state;

	if (m_charManager != nullptr) {
		if (m_currentState == State::GamestateMainMenu) {
			for (auto element : m_hud->GetElementsByTag("mainmenu"))
				element->SetVisible(true);

			for (auto element : m_hud->GetElementsByTag("losescreen"))
				element->SetVisible(false);

			for (auto element : m_hud->GetElementsByTag("ingame"))
				element->SetVisible(false);
		}
		else if (m_currentState == State::GamestatePlayerTurn) {
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
			m_charManager->SpawnEnemies();

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
		else if (m_currentState == State::GamestatePlayerLost) {
			for (auto element : m_hud->GetElementsByTag("losescreen"))
				element->SetVisible(true);

			for (auto element : m_hud->GetElementsByTag("ingame"))
				element->SetVisible(false);
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

	if (m_currentState == State::GamestateMainMenu) {
		for (auto element : m_hud->GetElementsByTag("mainmenu"))
			element->SetVisible(false);

		for (auto element : m_hud->GetElementsByTag("losescreen"))
			element->SetVisible(true);

		for (auto element : m_hud->GetElementsByTag("ingame"))
			element->SetVisible(false);
	}

	if (m_currentState == State::GamestatePlayerTurn)
		m_hud->SetVar("whoseturn", "Player's");
	else if (m_currentState == State::GamestateEnemyTurn)
		m_hud->SetVar("whoseturn", "Enemies'");
}

void StateManager::SetCamera(Camera* cam) {
	m_cam = cam;

	if (m_currentState == State::GamestateMainMenu) {
		m_cam->SetVerticleAngle(0.5);
		m_cam->ForceAngleUpdate();
	}
}