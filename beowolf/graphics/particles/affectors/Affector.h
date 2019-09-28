#ifndef AFFECTOR_H
#define AFFECTOR_H

#include "W_Common.h"
#include "Particle.h"

class Affector
{
public:
	virtual void Init(Particle* particle) {};
	virtual void Apply(Particle* particle, float delta) {};

private:
};

#endif