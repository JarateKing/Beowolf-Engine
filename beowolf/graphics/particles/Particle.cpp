#include "Particle.h"
#include "Emitter.h"
#include "W_RNG.h"

static const wolf::Vertex cubeVertices[] = {
	// Front
		{ -0.5f, -0.5f, 0.0f, 0, 0, 0, 0, 0.0f, 1.0f},
		{ -0.5f,  0.5f, 0.0f, 0, 0, 0, 0, 0.0f, 0.0f},
		{  0.5f,  0.5f, 0.0f, 0, 0, 0, 0, 1.0f, 0.0f},

		{  0.5f,  0.5f, 0.0f, 0, 0, 0, 0, 1.0f, 0.0f},
		{  0.5f, -0.5f, 0.0f, 0, 0, 0, 0, 1.0f, 1.0f},
		{ -0.5f, -0.5f, 0.0f, 0, 0, 0, 0, 0.0f, 1.0f},
};

Particle::Particle(int arrPos, wolf::Vertex* quad, Emitter* owner, float lifetime)
{
	m_pos = glm::vec3(0,0,0);
	m_scale = glm::vec3(1, 1, 1);
	m_color = glm::vec4(1, 1, 1, 1);

	m_arrPos = arrPos;
	m_quad = quad;
	m_owner = owner;

	m_lifetime = lifetime;
	m_duration = 0;
}

Particle::~Particle()
{
	// make vertex disappear
	for (int i = 0; i < 6; i++)
	{
		m_quad[i] = wolf::Vertex({0,0,0,0,0,0,0,0,0});
	}

	// add it back into the queue
	m_owner->AddToQueue(m_arrPos, m_quad);
}

void Particle::Update(float delta, glm::mat3 view)
{
	if (delta > 0)
	{
		m_duration += delta;
	}

	m_rot = glm::transpose(view);

	for (int i = 0; i < 6; i++)
	{
		glm::vec4 pos = glm::vec4(cubeVertices[i].x, cubeVertices[i].y, cubeVertices[i].z, 1);
		pos = glm::translate(m_pos) * glm::mat4(m_rot) * glm::scale(m_scale) * pos;
		m_quad[i] = wolf::Vertex({ pos.x, pos.y, pos.z, 0, 0, 0, 0, cubeVertices[i].u, cubeVertices[i].v });
		m_quad[i].setColor(m_color);
	}

	if (m_duration > m_lifetime && m_lifetime > 0)
	{
		delete this;
	}
}

void Particle::SetScale(float scale)
{
	m_scale = glm::vec3(scale, scale, scale);
}

void Particle::SetScale(glm::vec3 scale)
{
	m_scale = scale;
}

void Particle::SetColor(glm::vec3 color)
{
	m_color = glm::vec4(color.r, color.g, color.b, m_color.a);
}

void Particle::SetAlpha(float alpha)
{
	m_color.a = alpha;
}

void Particle::Translate(glm::vec3 pos)
{
	m_pos += pos;
}

float Particle::GetProgress()
{
	return m_duration / m_lifetime;
}