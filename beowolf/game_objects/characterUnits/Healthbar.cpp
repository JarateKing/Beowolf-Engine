#include "Healthbar.h"
#include "W_ResourceLoader.h"

const wolf::Vertex square[] = { wolf::Vertex({ -0.5, -0.5, 0.0f, 255, 255, 255, 255, 0, 0}),
								wolf::Vertex({ -0.5, 0.5, 0.0f, 255, 255, 255, 255, 0, 1}),
								wolf::Vertex({ 0.5, 0.5, 0.0f, 0, 0, 0, 255, 1, 1}),
								wolf::Vertex({ 0.5, 0.5, 0.0f, 0, 0, 0, 255, 1, 1}),
								wolf::Vertex({ 0.5, -0.5, 0.0f, 0, 0, 0, 255, 1, 0}),
								wolf::Vertex({ -0.5, -0.5, 0.0f, 255, 255, 255, 255, 0, 0})};

const glm::vec3 scale = glm::vec3(2.5, 0.15, 1.0);


Healthbar::Healthbar() {
	g_pProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("healthbar"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(&square, sizeof(wolf::Vertex) * 6);
	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

Healthbar::~Healthbar() {

}

void Healthbar::Update(const float& p_fDelta) {

}

void Healthbar::Render(const glm::mat4& view, const glm::mat4& proj) {
	// billboard healthbars
	glm::mat4 rot = glm::mat4(glm::transpose(glm::mat3(view)));

	g_pProgram->Bind();

	// Bind Uniforms
	g_pProgram->SetUniform("projection", proj);
	g_pProgram->SetUniform("view", view);
	g_pProgram->SetUniform("world", m_pos * rot * glm::scale(scale));
	g_pProgram->SetUniform("threshold", m_threshold); // what percent of the healthbar should be colored vs background

	// Set up source data
	g_pDecl->Bind();

	// Draw!
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Healthbar::SetPos(const glm::mat4& translation) {
	m_pos = translation;
}

void Healthbar::SetThreshold(const float& threshold) {
	m_threshold = threshold;
}
