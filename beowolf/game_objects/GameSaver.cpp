#include "GameSaver.h"
#include "W_Input.h"
#include "W_Math.h"

GameSaver::GameSaver(wolf::Hud* hud) {
	m_hud = hud;
}

GameSaver::~GameSaver() {}

void GameSaver::Update(float delta) {
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_Z)) {
		m_indicatorTime = 0.0f;
	}

	m_indicatorTime += delta;
	if (m_indicatorTime >= 1.0f)
		m_indicatorTime = 1.0f;

	for (auto element : m_hud->GetElementsByTag("save_indicator"))
		element->SetAlpha(wolf::Math::lerp(1.0f, 0.0f, wolf::Math::ease(m_indicatorTime)));
}

void GameSaver::SetInfo(CharacterInfoHub* hub, CharacterManager* manager, ScoreTracker* tracker, HexGrid* grid) {
	m_hub = hub;
	m_manager = manager;
	m_tracker = tracker;
	m_grid = grid;
}