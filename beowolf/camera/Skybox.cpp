#include "Skybox.h"
#include "W_ResourceLoader.h"

Skybox::Skybox()
{
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("skybox"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(planeVertices, sizeof(wolf::Vertex) * 36);
	m_tex = wolf::TextureManager::CreateTexture(wolf::ResourceLoader::Instance().getTexture("skybox.tga"), false);

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

Skybox::~Skybox()
{

}

void Skybox::SetPos(glm::vec3 pos) {
	m_pos = pos;
}

void Skybox::Render(glm::mat4 projView, wolf::RenderFilterType type)
{
	if (type == wolf::RenderFilterOpaque) {
		g_dProgram->Bind();
		m_tex->Bind();

		// Bind Uniforms
		g_dProgram->SetUniform("projection", projView * glm::translate(m_pos) * glm::rotate(-70.0f, glm::vec3(0, 1, 0)));

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}