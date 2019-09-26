#ifndef AFFECTORRECOLOR_H
#define AFFECTORRECOLOR_H

#include "W_Common.h"
#include "Affector.h"

class AffectorRecolor: public Affector
{
public:
	AffectorRecolor(glm::vec3 startClr, glm::vec3 endClr);
	~AffectorRecolor();
	void Apply(Particle* particle, float delta);

private:
	glm::vec3 m_startClr;
	glm::vec3 m_endClr;
};

#endif