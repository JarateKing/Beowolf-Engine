#include "GameSaver.h"
#include "W_Input.h"

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
		element->SetAlpha(1.0 - m_indicatorTime);
}