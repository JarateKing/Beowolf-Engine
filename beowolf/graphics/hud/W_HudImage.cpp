#include "W_HudImage.h"
#include "W_ResourceLoader.h"

const wolf::Vertex square[] = { wolf::Vertex({ 0, 0, 1.0f, 1, 1, 1, 1, 0, 1}),
								wolf::Vertex({ 0, 1, 1.0f, 1, 1, 1, 1, 0, 0}),
								wolf::Vertex({ 1, 1, 1.0f, 1, 1, 1, 1, 1, 0}),
								wolf::Vertex({ 1, 1, 1.0f, 1, 1, 1, 1, 1, 0}),
								wolf::Vertex({ 1, 0, 1.0f, 1, 1, 1, 1, 1, 1}),
								wolf::Vertex({ 0, 0, 1.0f, 1, 1, 1, 1, 0, 1})};

namespace wolf {
	HudImage::HudImage(std::string image) {
		m_tex = TextureManager::CreateTexture(ResourceLoader::Instance().getTexture(image), false);

		g_pProgram = ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("hud_texture"));
		g_pVB = BufferManager::CreateVertexBuffer(&square, sizeof(Vertex) * 6);
		g_pDecl = new VertexDeclaration();
		g_pDecl->Begin();
		Vertex::applyAttributes(g_pDecl);
		g_pDecl->SetVertexBuffer(g_pVB);
		g_pDecl->End();
	}

	HudImage::~HudImage() {

	}

	void HudImage::Update(float p_fDelta) {

	}

	void HudImage::Render(glm::mat4 proj) {
		if (m_isVisible) {
			g_pProgram->Bind();
			m_tex->Bind();

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
}