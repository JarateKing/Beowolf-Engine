#include "ScoreTracker.h"
#include <fstream>

ScoreTracker::ScoreTracker(wolf::Hud* hud) {
	m_hud = hud;

	// if there is no highscores file, make a stub one
	if (!std::ifstream("highscores.save").good()) {
		std::ofstream outfile;
		outfile.open("highscores.save", std::ios::binary);
		int score = 0;
		for (int i = 0; i < 5; i++)
			outfile.write((char*)&score, sizeof(int));
		outfile.close();
	}

	// load in highscores
	std::ifstream infile("highscores.save", std::ifstream::binary);
	if (infile.good()) {
		for (int i = 0; i < 5; i++) {
			infile.read((char*)&m_highscores[i], sizeof(int));
		}
	}

	ApplyToHud();
}

ScoreTracker::~ScoreTracker() {
	
}

void ScoreTracker::SetScore(const int& score) {
	m_score = score;
	ApplyToHud();
}

void ScoreTracker::AddScore(const int& score) {
	m_score += score;
	ApplyToHud();
}

int ScoreTracker::GetScore() {
	return m_score;
}

void ScoreTracker::ApplyToHud() {
	m_hud->SetVar("score", std::to_string(m_score));

	for (int i = 0; i < 5; i++)
		m_hud->SetVar("highscore" + std::to_string(i + 1), std::to_string(m_highscores[i]));
}

void ScoreTracker::ApplyHighscore() {
	// apply new highscores
	int current = m_score;
	for (int i = 0; i < 5; i++)
		if (current > m_highscores[i])
			std::swap(m_highscores[i], current);

	// save scores
	std::ofstream outfile;
	if (outfile.good()) {
		std::ofstream outfile;
		outfile.open("highscores.save", std::ios::binary);
		for (int i = 0; i < 5; i++)
			outfile.write((char*)&m_highscores[i], sizeof(int));
	}
	outfile.close();

	ApplyToHud();
}
