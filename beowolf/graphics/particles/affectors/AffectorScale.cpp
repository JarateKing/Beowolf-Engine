#include "AffectorScale.h"

AffectorScale::AffectorScale(float startSize, float endSize)
{
	m_startSize = startSize;
	m_endSize = endSize;
}

AffectorScale::~AffectorScale()
{

}

void AffectorScale::Apply(Particle* particle, float delta)
{
	float percent = particle->GetProgress();
	float size = m_startSize * (1 - percent) + m_endSize * (percent);
	particle->SetScale(size);
}