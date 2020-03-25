#ifndef PARTICLE_H
#define PARTICLE_H

#include "W_Common.h"

class Emitter;

class Particle
{
public:
	Particle(int arrPos, wolf::Vertex* quad, Emitter* owner, float lifetime);
	~Particle();
	void Update(float delta, glm::mat3 view);

	void SetScale(float scale);
	void SetScale(glm::vec3 scale);
	void SetColor(glm::vec3 color);
	void SetAlpha(float alpha);
	void SetAnim(int frames, int framerate);

	void Translate(glm::vec3 pos);

	float GetProgress();

private:
	glm::vec3 m_pos;
	glm::mat3 m_rot;
	glm::vec3 m_scale;
	glm::vec4 m_color;

	int m_arrPos;
	wolf::Vertex* m_quad;
	Emitter* m_owner;
	float m_lifetime;
	float m_duration;
	int m_frames = 1;
	int m_framerate = 60;
	int m_currentframe = 0;
};

#endif