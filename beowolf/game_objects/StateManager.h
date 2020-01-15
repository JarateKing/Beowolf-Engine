class StateManager
{
public:
	static StateManager& getInstance()
	{
		static StateManager instance;
		return instance;
	}

	int GetState();

private:
	StateManager() {}

public:
	StateManager(StateManager const&) = delete;
	void operator=(StateManager const&) = delete;

private:

};