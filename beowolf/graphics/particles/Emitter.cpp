#include "Emitter.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"

Emitter::Emitter(int max, float duration, float rate, std::string texture)
{
	m_max = max;
	m_duration = duration;
	m_timeActive = 0;

	m_spawnrate = rate;
	m_spawnrateOverflow = 0;

	m_isAdditive = false;

	m_lifespanMin = 1.0f;
	m_lifespanMax = 1.0f;

	m_particles = new Particle*[m_max];
	m_quads = new wolf::Vertex[m_max * 6];
	for (int i = 0; i < m_max; i++)
	{
		m_availableSlots.push(ParticlePointerPair({i, m_quads + i * 6 }));
		m_particles[i] = NULL;
		for (int j = 0; j < 6; j++)
		{
			m_quads[i*6+j] = wolf::Vertex({ 0, 0, 0, 0, 0, 0, 0, 0, 0 });
		}
	}

	g_pProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("particle"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(m_quads, sizeof(wolf::Vertex) * m_max * 6);
	g_pTexture = wolf::CreateTextureFromDDS(texture);

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

Emitter::~Emitter()
{
	for (int i = 0; i < m_max; i++)
	{
		delete m_particles[i];
	}
	delete[] m_particles;

	for (int i = 0; i < m_affectors.size(); i++)
	{
		delete m_affectors[i];
	}
	delete[] m_quads;

	wolf::BufferManager::DestroyBuffer(g_pVB);
	delete g_pDecl;
}

void Emitter::Update(float delta, glm::mat3 view)
{
	if (delta > 0)
	{
		m_timeActive += delta;
		m_spawnrateOverflow += delta;
	}

	while (!m_availableSlots.empty() && (m_timeActive < m_duration || m_duration < 0) && (m_spawnrateOverflow >= m_spawnrate))
	{
		m_spawnrateOverflow -= m_spawnrate;
		ParticlePointerPair ppp = m_availableSlots.front();
		m_availableSlots.pop();

		m_particles[ppp.particle] = new Particle(ppp.particle, ppp.quad, this, wolf::RNG::GetRandom(m_lifespanMin, m_lifespanMax));
		m_particles[ppp.particle]->Translate(m_position);
		m_particles[ppp.particle]->SetAnim(m_animationFrames, m_animationFramerate);

		for (int i = 0; i < m_affectors.size(); i++)
		{
			m_affectors[i]->Init(m_particles[ppp.particle]);
		}
	}

	for (int i = 0; i < m_max; i++)
	{
		if (m_particles[i] != NULL)
		{
			for (int j = 0; j < m_affectors.size(); j++)
			{
				m_affectors[j]->Apply(m_particles[i], delta);
			}

			m_particles[i]->Update(delta, view);
		}
	}

	// recreate the buffer
	wolf::BufferManager::DestroyBuffer(g_pVB);
	g_pVB = wolf::BufferManager::CreateVertexBuffer(m_quads, sizeof(wolf::Vertex) * m_max * 6);

	delete g_pDecl;
	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

void Emitter::Render(glm::mat4 projview, wolf::RenderFilterType type)
{
	if ((m_isAdditive && type == wolf::RenderFilterAdditive) || (!m_isAdditive && type == wolf::RenderFilterTransparent))
	{
		g_pProgram->Bind();
		glBindTexture(GL_TEXTURE_2D, g_pTexture);

		// Bind Uniforms
		g_pProgram->SetUniform("projection", projview);
		g_pProgram->SetUniform("tex", 0);

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, m_max * 6);
	}
}

void Emitter::AddToQueue(int index, wolf::Vertex* ptr)
{
	m_particles[index] = NULL;
	m_availableSlots.push(ParticlePointerPair({ index, ptr }));
}

void Emitter::AddAffector(Affector* affector)
{
	m_affectors.push_back(affector);
}

bool Emitter::RemoveAffector(Affector* affector)
{
	for (int i = 0; i < m_affectors.size(); i++)
	{
		if (m_affectors[i] == affector)
		{
			m_affectors.erase(m_affectors.begin() + i);
			return true;
		}
	}

	return false;
}

void Emitter::Translate(glm::vec3 pos)
{
	m_position += pos;
}

void Emitter::SetAdditive(bool value)
{
	m_isAdditive = value;
}

void Emitter::SetLifespan(float min, float max)
{
	m_lifespanMin = min;
	m_lifespanMax = max;
}

void Emitter::SetAnimatedTexture(int frames, int framerate)
{
	m_animationFrames = frames;
	m_animationFramerate = framerate;
}