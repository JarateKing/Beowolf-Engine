#ifndef AFFECTORRANDOMPOS_H
#define AFFECTORRANDOMPOS_H

#include "W_Common.h"
#include "Affector.h"

class AffectorRandomPos: public Affector
{
public:
	AffectorRandomPos(float radius);
	~AffectorRandomPos();
	void Init(Particle* particle);

private:
	float m_radius;
};

#endif