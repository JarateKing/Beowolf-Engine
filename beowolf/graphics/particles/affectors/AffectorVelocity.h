#ifndef AFFECTORVELOCITY_H
#define AFFECTORVELOCITY_H

#include "W_Common.h"
#include "Affector.h"

class AffectorVelocity: public Affector
{
public:
	AffectorVelocity(glm::vec3 startVelocity, glm::vec3 endVelocity);
	~AffectorVelocity();
	void Apply(Particle* particle, float delta);

private:
	glm::vec3 m_startVelocity;
	glm::vec3 m_endVelocity;
};

#endif