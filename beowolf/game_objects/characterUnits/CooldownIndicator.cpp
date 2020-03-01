#include "CooldownIndicator.h"
#include "W_ResourceLoader.h"
#include "W_Math.h"

const wolf::Vertex square[] = { wolf::Vertex({ -0.5, -0.5, 1.0f, 1, 1, 1, 1, 1, 0}),
								wolf::Vertex({ -0.5, 0.5, 1.0f, 1, 1, 1, 1, 1, 1}),
								wolf::Vertex({ 0.5, 0.5, 1.0f, 1, 1, 1, 1, 0, 1}),
								wolf::Vertex({ 0.5, 0.5, 1.0f, 1, 1, 1, 1, 0, 1}),
								wolf::Vertex({ 0.5, -0.5, 1.0f, 1, 1, 1, 1, 0, 0}),
								wolf::Vertex({ -0.5, -0.5, 1.0f, 1, 1, 1, 1, 1, 0}) };

const glm::vec3 scale = glm::vec3(0.75 * 16.0 / 9.0, 0.75, 0.75);


CooldownIndicator::CooldownIndicator() {
	m_tex = wolf::TextureManager::CreateTexture(wolf::ResourceLoader::Instance().getTexture("hud/cooldown_indicator.tga"), false);
	g_pProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("cooldown"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(&square, sizeof(wolf::Vertex) * 6);
	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();

	ResetThreshold();
}

CooldownIndicator::~CooldownIndicator() {

}

void CooldownIndicator::Update(float p_fDelta) {
	if (m_threshold != m_targetThreshold) {
		m_targetThresholdTime += p_fDelta * 0.5;
		if (m_targetThresholdTime > 1.0)
			m_targetThresholdTime = 1.0;

		m_threshold = wolf::Math::lerp(m_startThreshold, m_targetThreshold, wolf::Math::easeOut(m_targetThresholdTime));
	}
}

void CooldownIndicator::Render(glm::mat4 view, glm::mat4 proj) {
	if (m_threshold < 0.99f) {
		glm::mat4 rot = glm::mat4(glm::transpose(glm::mat3(view)));

		g_pProgram->Bind();
		m_tex->Bind();

		// Bind Uniforms
		g_pProgram->SetUniform("projection", proj);
		g_pProgram->SetUniform("view", view);
		g_pProgram->SetUniform("tex", 0);
		g_pProgram->SetUniform("world", m_pos * rot * glm::scale(scale));
		g_pProgram->SetUniform("threshold", m_threshold);

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void CooldownIndicator::SetPos(glm::mat4 translation) {
	m_pos = translation;
}

void CooldownIndicator::SetThreshold(float threshold, bool forceSet) {
	m_targetThresholdTime = 0.0f;
	m_targetThreshold = threshold;

	if (forceSet) {
		m_startThreshold = threshold;
		m_threshold = threshold;
	}
	else {
		m_startThreshold = m_threshold;
	}
}

void CooldownIndicator::ResetThreshold() {
	m_threshold = 1.0f;
	m_targetThreshold = 1.0f;
	m_targetThresholdTime = 0.0f;
	m_startThreshold = 1.0f;
}
