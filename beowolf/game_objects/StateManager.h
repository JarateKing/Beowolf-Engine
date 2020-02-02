#include "States.h"
#include "characterUnits/CharacterManager.h"
#include "W_Hud.h"

class StateManager
{
public:
	static StateManager& getInstance()
	{
		static StateManager instance;
		return instance;
	}

	void Update(float delta);

	State GetState();
	void SetState(State state);
	void SetCharacterManager(CharacterManager* charMan);
	void SetHud(wolf::Hud* hud);

private:
	StateManager() {
		SetState(State::GamestateMainMenu);
	}

public:
	StateManager(StateManager const&) = delete;
	void operator=(StateManager const&) = delete;

private:
	State m_currentState;
	CharacterManager* m_charManager;
	wolf::Hud* m_hud;
};