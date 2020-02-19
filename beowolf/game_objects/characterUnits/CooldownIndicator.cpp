#include "CooldownIndicator.h"
#include "W_ResourceLoader.h"

const wolf::Vertex square[] = { wolf::Vertex({ 0, 0, 1.0f, 1, 1, 1, 1, 1, 0}),
								wolf::Vertex({ 0, 1, 1.0f, 1, 1, 1, 1, 1, 1}),
								wolf::Vertex({ 1, 1, 1.0f, 1, 1, 1, 1, 0, 1}),
								wolf::Vertex({ 1, 1, 1.0f, 1, 1, 1, 1, 0, 1}),
								wolf::Vertex({ 1, 0, 1.0f, 1, 1, 1, 1, 0, 0}),
								wolf::Vertex({ 0, 0, 1.0f, 1, 1, 1, 1, 1, 0}) };

const glm::vec3 scale = glm::vec3(1.0 * 16.0 / 9.0, 1.0, 1.0);


CooldownIndicator::CooldownIndicator() {
	m_tex = wolf::TextureManager::CreateTexture(wolf::ResourceLoader::Instance().getTexture("hud/cooldown_indicator.tga"), false);
	g_pProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("cooldown"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(&square, sizeof(wolf::Vertex) * 6);
	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

CooldownIndicator::~CooldownIndicator() {

}

void CooldownIndicator::Update(float p_fDelta) {

}

void CooldownIndicator::Render(glm::mat4 view, glm::mat4 proj) {
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

void CooldownIndicator::SetPos(glm::mat4 translation) {
	m_pos = translation;
}

void CooldownIndicator::SetThreshold(float threshold) {
	m_threshold = threshold;
}
