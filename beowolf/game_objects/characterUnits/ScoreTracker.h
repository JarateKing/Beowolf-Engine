#ifndef SCORETRACKER_H
#define SCORETRACKER_H

#include "W_Common.h"
#include "W_Hud.h"


class ScoreTracker
{
public:
	ScoreTracker(wolf::Hud* hud);
	~ScoreTracker();

	void SetScore(int score);
	void AddScore(int score);
	int GetScore(int score);
private:
	int m_score = 0;
	wolf::Hud* m_hud;
};

#endif