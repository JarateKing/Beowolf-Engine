#include "W_HudGradient.h"
#include "W_ResourceLoader.h"

namespace wolf {
	HudGradient::HudGradient(const glm::vec4& topleft, const glm::vec4& topright, const glm::vec4& bottomleft, const glm::vec4& bottomright) {
		const wolf::Vertex square[] = { wolf::Vertex({ 0, 0, 1.0f, (GLubyte)(topleft.r * 255), (GLubyte)(topleft.g * 255), (GLubyte)(topleft.b * 255), (GLubyte)(topleft.a * 255), 0, 0}),
								        wolf::Vertex({ 0, 1, 1.0f, (GLubyte)(bottomleft.r * 255), (GLubyte)(bottomleft.g * 255), (GLubyte)(bottomleft.b * 255), (GLubyte)(bottomleft.a * 255), 0, 1}),
								        wolf::Vertex({ 1, 1, 1.0f, (GLubyte)(bottomright.r * 255), (GLubyte)(bottomright.g * 255), (GLubyte)(bottomright.b * 255), (GLubyte)(bottomright.a * 255), 1, 1}),
								        wolf::Vertex({ 1, 1, 1.0f, (GLubyte)(bottomright.r * 255), (GLubyte)(bottomright.g * 255), (GLubyte)(bottomright.b * 255), (GLubyte)(bottomright.a * 255), 1, 1}),
								        wolf::Vertex({ 1, 0, 1.0f, (GLubyte)(topright.r * 255), (GLubyte)(topright.g * 255), (GLubyte)(topright.b * 255), (GLubyte)(topright.a * 255), 1, 0}),
								        wolf::Vertex({ 0, 0, 1.0f, (GLubyte)(topleft.r * 255), (GLubyte)(topleft.g * 255), (GLubyte)(topleft.b * 255), (GLubyte)(topleft.a * 255), 0, 0}) };

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

	void HudGradient::Update(const float& p_fDelta) {

	}

	void HudGradient::Render(const glm::mat4& proj) {
		if (m_isVisible) {
			g_pProgram->Bind();

			// Bind Uniforms
			g_pProgram->SetUniform("projection", proj);
			g_pProgram->SetUniform("world", m_world);
			g_pProgram->SetUniform("tex", 0);
			g_pProgram->SetUniform("alpha", m_alpha);

			// Set up source data
			g_pDecl->Bind();

			// Draw!
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}