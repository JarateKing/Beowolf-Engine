#include "AffectorRecolor.h"

AffectorRecolor::AffectorRecolor(glm::vec3 startClr, glm::vec3 endClr)
{
	m_startClr = startClr;
	m_endClr = endClr;
}

AffectorRecolor::~AffectorRecolor()
{

}

void AffectorRecolor::Apply(Particle* particle, float delta)
{
	float percent = particle->GetProgress();
	glm::vec3 color = m_startClr * (1 - percent) + m_endClr * (percent);
	particle->SetColor(color);
}