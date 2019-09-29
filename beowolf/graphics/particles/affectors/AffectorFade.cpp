#include "AffectorFade.h"

AffectorFade::AffectorFade(float startAlpha, float endAlpha)
{
	m_startAlpha = startAlpha;
	m_endAlpha = endAlpha;
}

AffectorFade::~AffectorFade()
{

}

void AffectorFade::Apply(Particle* particle, float delta)
{
	float percent = particle->GetProgress();
	float alpha = m_startAlpha * (1 - percent) + m_endAlpha * (percent);
	particle->SetAlpha(alpha);
}