#include "AffectorBounce.h"

AffectorBounce::AffectorBounce(float magnitude, float period)
{
	m_magnitude = magnitude;
	m_period = period;
}

AffectorBounce::~AffectorBounce()
{

}

void AffectorBounce::Apply(Particle* particle, float delta)
{
	m_cumulativeDelta += delta;
	float offset = m_magnitude * std::sin(m_cumulativeDelta / m_period);
	particle->Translate(glm::vec3(0, offset * 0.01f, 0));
}