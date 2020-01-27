#include "ScoreTracker.h"

ScoreTracker::ScoreTracker(wolf::Hud* hud) {
	m_hud = hud;
	m_hud->SetVar("score", "0");
}

ScoreTracker::~ScoreTracker() {

}

void ScoreTracker::SetScore(int score) {
	m_score = score;
	ApplyToHud();
}

void ScoreTracker::AddScore(int score) {
	m_score += score;
	ApplyToHud();
}

int ScoreTracker::GetScore(int score) {
	return m_score;
}

void ScoreTracker::ApplyToHud() {
	m_hud->SetVar("score", std::to_string(m_score));
}
