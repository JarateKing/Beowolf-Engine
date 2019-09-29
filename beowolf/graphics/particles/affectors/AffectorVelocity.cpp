#include "AffectorVelocity.h"

AffectorVelocity::AffectorVelocity(glm::vec3 startVelocity, glm::vec3 endVelocity)
{
	m_startVelocity = startVelocity;
	m_endVelocity = endVelocity;
}

AffectorVelocity::~AffectorVelocity()
{

}

void AffectorVelocity::Apply(Particle* particle, float delta)
{
	float percent = particle->GetProgress();
	glm::vec3 velocity = m_startVelocity * (1 - percent) + m_endVelocity * (percent);
	velocity *= delta;
	particle->Translate(velocity);
}