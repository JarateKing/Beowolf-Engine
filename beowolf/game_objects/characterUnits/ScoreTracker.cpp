#include "ScoreTracker.h"

ScoreTracker::ScoreTracker(wolf::Hud* hud) {
	m_hud = hud;
}

ScoreTracker::~ScoreTracker() {

}

void ScoreTracker::SetScore(int score) {
	m_score = score;
}

void ScoreTracker::AddScore(int score) {
	m_score += score;
}

int ScoreTracker::GetScore(int score) {
	return m_score;
}
