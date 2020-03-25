#ifndef AFFECTORBOUNCE_H
#define AFFECTORBOUNCE_H

#include "W_Common.h"
#include "Affector.h"

class AffectorBounce: public Affector
{
public:
	AffectorBounce(float magnitude, float period);
	~AffectorBounce();
	void Apply(Particle* particle, float delta);

private:
	float m_cumulativeDelta = 0;
	float m_magnitude;
	float m_period;
};

#endif