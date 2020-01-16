#include "States.h"

class StateManager
{
public:
	static StateManager& getInstance()
	{
		static StateManager instance;
		return instance;
	}

	State GetState();
	void SetState(State state);

private:
	StateManager() {
		m_currentState = State::GamestatePlayerTurn;
	}

public:
	StateManager(StateManager const&) = delete;
	void operator=(StateManager const&) = delete;

private:
	State m_currentState;
};