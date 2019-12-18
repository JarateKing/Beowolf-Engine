#include "W_HudGradient.h"
#include "W_ResourceLoader.h"

namespace wolf {
	HudGradient::HudGradient(glm::vec4 topleft, glm::vec4 topright, glm::vec4 bottomleft, glm::vec4 bottomright) {
		wolf::Vertex square[] = { wolf::Vertex({ 0, 0, 1.0f, topleft.r, topleft.g, topleft.b, topleft.a, 0, 0}),
								  wolf::Vertex({ 0, 1, 1.0f, bottomleft.r, bottomleft.g, bottomleft.b, bottomleft.a, 0, 1}),
								  wolf::Vertex({ 1, 1, 1.0f, bottomright.r, bottomright.g, bottomright.b, bottomright.a, 1, 1}),
								  wolf::Vertex({ 1, 1, 1.0f, bottomright.r, bottomright.g, bottomright.b, bottomright.a, 1, 1}),
								  wolf::Vertex({ 1, 0, 1.0f, topright.r, topright.g, topright.b, topright.a, 1, 0}),
								  wolf::Vertex({ 0, 0, 1.0f, topleft.r, topleft.g, topleft.b, topleft.a, 0, 0}) };

		g_pProgram = ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("hud_gradient"));
		g_pVB = BufferManager::CreateVertexBuffer(&square, sizeof(Vertex) * 6);
		g_pDecl = new VertexDeclaration();
		g_pDecl->Begin();
		Vertex::applyAttributes(g_pDecl);
		g_pDecl->SetVertexBuffer(g_pVB);
		g_pDecl->End();
	}

	HudGradient::~HudGradient() {

	}

	void HudGradient::Update(float p_fDelta) {

	}

	void HudGradient::Render(glm::mat4 proj) {
		g_pProgram->Bind();

		// Bind Uniforms
		g_pProgram->SetUniform("projection", proj);
		g_pProgram->SetUniform("world", m_world);
		g_pProgram->SetUniform("tex", 0);

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}