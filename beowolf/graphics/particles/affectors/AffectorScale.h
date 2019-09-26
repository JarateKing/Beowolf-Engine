#ifndef AFFECTORSCALE_H
#define AFFECTORSCALE_H

#include "W_Common.h"
#include "Affector.h"

class AffectorScale: public Affector
{
public:
	AffectorScale(float startSize, float endSize);
	~AffectorScale();
	void Apply(Particle* particle, float delta);

private:
	float m_startSize;
	float m_endSize;
};

#endif