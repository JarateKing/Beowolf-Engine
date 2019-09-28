#include "AffectorRandomPos.h"
#include "W_RNG.h"

AffectorRandomPos::AffectorRandomPos(float radius)
{
	m_radius = radius;
}

AffectorRandomPos::~AffectorRandomPos()
{

}

void AffectorRandomPos::Init(Particle* particle)
{
	glm::vec3 offset = glm::vec3(m_radius, 0, 0) * glm::quat(glm::vec3(wolf::RNG::GetRandom(0.0f, 360.0f), wolf::RNG::GetRandom(0.0f, 360.0f), wolf::RNG::GetRandom(0.0f, 360.0f)));
	particle->Translate(offset);
}