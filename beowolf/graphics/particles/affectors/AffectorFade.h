#ifndef AFFECTORFADE_H
#define AFFECTORFADE_H

#include "W_Common.h"
#include "Affector.h"

class AffectorFade: public Affector
{
public:
	AffectorFade(float startAlpha, float endAlpha);
	~AffectorFade();
	void Apply(Particle* particle, float delta);

private:
	float m_startAlpha;
	float m_endAlpha;
};

#endif