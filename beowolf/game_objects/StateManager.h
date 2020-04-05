#include "States.h"
#include "characterUnits/CharacterManager.h"
#include "W_Hud.h"
#include "camera/Camera.h"
#include "sound/W_SoundEngine.h"

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
	void SetSoundEngine(wolf::SoundEngine* soundEng);
	void SetHud(wolf::Hud* hud);
	void SetCamera(Camera* cam);
	void SetScoreTracker(ScoreTracker* tracker);

private:
	StateManager() {
		m_currentState = State::GamestateMainMenu;
	}

public:
	StateManager(StateManager const&) = delete;
	void operator=(StateManager const&) = delete;

private:
	State m_currentState;
	CharacterManager* m_charManager;
	wolf::SoundEngine* m_soundEngine;
	wolf::Hud* m_hud;
	Camera* m_cam;
	ScoreTracker* m_scoreTracker;
};