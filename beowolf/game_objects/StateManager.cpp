#include "StateManager.h"
#include <iostream>
#include <list>
#include "W_HudButton.h"
#include "W_Math.h"
#include <vector>
#include "W_Input.h"
#include "W_Keybind.h"

void StateManager::Update(float delta) {
	static float time = 0;
	static bool movingcamera = false;

	// move-in camera when first starting the game
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

	// check for state change conditions
	if (m_charManager != nullptr && m_hud != nullptr) {
		if (m_currentState == State::GamestateMainMenu) {
			if (wolf::Keybind::Instance().getBind("startgame") || ((wolf::HudButton*)m_hud->GetElement("MM_Start_Button"))->IsClicked() || (((wolf::HudButton*)m_hud->GetElement("MM_Load_Button"))->IsClicked() && m_hud->GetElement("MM_Load_Button")->GetVisible())) {
				SetState(State::GamestatePlayerTurn);

				for (auto element : m_hud->GetElementsByTag("mainmenu"))
					element->SetVisible(false);

				for (auto element : m_hud->GetElementsByTag("ingame")) {
					element->SetVisible(true);
					element->SetAlpha(0);
				}

				if (m_cam != nullptr)
					movingcamera = true;
				m_soundEngine->PlayBasicSound("start_jingle");
				m_soundEngine->UpdateSystem();
			}

			if (wolf::Keybind::Instance().getBind("scoreboard") || ((wolf::HudButton*)m_hud->GetElement("MM_Scoreboard_Button"))->IsClicked()) {
				SetState(State::GamestateScoreboard);
			}
		}
		else if (m_currentState == State::GamestatePlayerLost) {
			if (wolf::Keybind::Instance().getBind("startgame") || ((wolf::HudButton*)m_hud->GetElement("LS_Restart_Button"))->IsClicked()) {
				SetState(State::GamestatePlayerTurn);

				for (auto element : m_hud->GetElementsByTag("losescreen"))
					element->SetVisible(false);

				for (auto element : m_hud->GetElementsByTag("ingame"))
					element->SetVisible(true);
			}
		}
		else if (m_currentState == State::GamestatePlayerTurn) {
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
			{
				SetState(State::GamestateEnemyTurn);
				m_soundEngine->PlayBasicSound("movement2");
			}
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
				{
					SetState(State::GamestatePlayerLost);
					m_soundEngine->PlayBasicSound("lose_jingle");
				}
				else
				{
					SetState(State::GamestatePlayerTurn);
					m_soundEngine->PauseSound("movement2");
				}
			}
		}
		else if (m_currentState == State::GamestateScoreboard) {
			if (wolf::Keybind::Instance().getBind("leavescoreboard") || ((wolf::HudButton*)m_hud->GetElement("Scoreboard_Back_Button"))->IsClicked()) {
				SetState(State::GamestateMainMenu);
			}
		}
	}
}

State StateManager::GetState() {
	return m_currentState;
}

void StateManager::SetState(const State& state) {
	m_currentState = state;

	// apply new state effects
	if (m_charManager != nullptr) {
		if (m_currentState == State::GamestateMainMenu) {
			for (auto element : m_hud->GetElementsByTag("mainmenu"))
				element->SetVisible(true);

			for (auto element : m_hud->GetElementsByTag("ingame"))
				element->SetVisible(false);

			for (auto element : m_hud->GetElementsByTag("scoreboard"))
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
				it->UpdateCooldown();
			}
			auto enemies = m_charManager->getEnemies();
			for (auto it = enemies->begin(); it != enemies->end(); it++) {
				it->setHasMoved(false);
			}

			if (m_hud != nullptr)
				m_hud->SetVar("whoseturn", "Enemies'");
		
			m_charManager->MoveEnemies();
		}
		else if (m_currentState == State::GamestatePlayerLost) {
			for (auto element : m_hud->GetElementsByTag("losescreen"))
				element->SetVisible(true);

			for (auto element : m_hud->GetElementsByTag("ingame"))
				element->SetVisible(false);

			// reset healthbar positions
			for (auto element : m_hud->GetElementsByTag("hpbar2"))
				element->SetY(element->GetY() + 60);
			for (auto element : m_hud->GetElementsByTag("hpbar3"))
				element->SetY(element->GetY() + 120);

			m_scoreTracker->ApplyHighscore();
		}
		else if (m_currentState == State::GamestateScoreboard) {
			for (auto element : m_hud->GetElementsByTag("mainmenu"))
				element->SetVisible(false);
			
			for (auto element : m_hud->GetElementsByTag("scoreboard"))
				element->SetVisible(true);
		}
	}
}

void StateManager::SetCharacterManager(CharacterManager* charMan) {
	if (m_charManager == charMan)
		std::cout << "Note: character manager already tracked by state manager\n";

	m_charManager = charMan;
}

void StateManager::SetSoundEngine(wolf::SoundEngine* soundEng) {
	if (m_soundEngine == soundEng)
		std::cout << "Note: sound engine already tracked by state manager\n";

	m_soundEngine = soundEng;
}

void StateManager::SetHud(wolf::Hud* hud) {
	if (m_hud == hud)
		std::cout << "Note: hud already tracked by state manager\n";

	m_hud = hud;

	// set default hud state when hud is first set
	if (m_currentState == State::GamestateMainMenu) {
		for (auto element : m_hud->GetElementsByTag("mainmenu"))
			element->SetVisible(true);

		for (auto element : m_hud->GetElementsByTag("losescreen"))
			element->SetVisible(false);

		for (auto element : m_hud->GetElementsByTag("ingame"))
			element->SetVisible(false);

		for (auto element : m_hud->GetElementsByTag("scoreboard"))
			element->SetVisible(false);
	}
	else if (m_currentState == State::GamestatePlayerTurn) {
		m_hud->SetVar("whoseturn", "Player's");
	}
	else if (m_currentState == State::GamestateEnemyTurn) {
		m_hud->SetVar("whoseturn", "Enemies'");
	}
}

void StateManager::SetCamera(Camera* cam) {
	m_cam = cam;

	// change camera angle upwards
	if (m_currentState == State::GamestateMainMenu) {
		m_cam->SetVerticleAngle(0.5);
		m_cam->ForceAngleUpdate();
	}
}

void StateManager::SetScoreTracker(ScoreTracker* tracker) {
	m_scoreTracker = tracker;
}