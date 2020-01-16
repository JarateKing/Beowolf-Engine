#include "States.h"
#include "characterUnits/CharacterManager.h"

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

private:
	StateManager() {
		m_currentState = State::GamestatePlayerTurn;
	}

public:
	StateManager(StateManager const&) = delete;
	void operator=(StateManager const&) = delete;

private:
	State m_currentState;
	CharacterManager* m_charManager;
};