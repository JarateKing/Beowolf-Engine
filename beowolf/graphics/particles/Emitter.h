#ifndef EMITTER_H
#define EMITTER_H

#include "W_Common.h"
#include "Particle.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include <queue>
#include "Affector.h"

struct ParticlePointerPair
{
	int particle;
	wolf::Vertex* quad;
};

class Emitter
{
public:
	Emitter(int max, float duration, float rate, std::string texture);
	~Emitter();
	virtual void Update(float delta, glm::mat3 view);
	virtual void Render(glm::mat4 projview, wolf::RenderFilterType type);
	void AddToQueue(int index, wolf::Vertex* ptr);
	void AddAffector(Affector* affector);
	bool RemoveAffector(Affector* affector);

	void Translate(glm::vec3 pos);
	void SetAdditive(bool value);
	void SetLifespan(float min, float max);
	void SetAnimatedTexture(int frames, int framerate);

private:
	int m_max;
	float m_duration;
	float m_timeActive;
	float m_spawnrate;
	float m_spawnrateOverflow;
	float m_lifespanMin;
	float m_lifespanMax;
	int m_animationFrames = 1;
	int m_animationFramerate = 60;
	bool m_isAdditive;
	glm::vec3 m_position;
	std::queue<ParticlePointerPair> m_availableSlots;
	wolf::Vertex* m_quads;
	Particle** m_particles;
	std::vector<Affector*> m_affectors;

	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_pProgram;
	GLuint g_pTexture;
};

#endif