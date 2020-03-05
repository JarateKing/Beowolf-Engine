#include "Water.h"
#include "W_ResourceLoader.h"

Water::Water()
{
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("water"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(planeVertices, sizeof(wolf::Vertex) * 6);
	m_tex = wolf::TextureManager::CreateTexture(wolf::ResourceLoader::Instance().getTexture("water.tga"), false);
	m_tex->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

Water::~Water()
{

}

void Water::Update(float delta) {
	m_time += delta;
}

void Water::SetPos(glm::vec3 pos) {
	m_pos = glm::vec3(pos.x, 0, pos.z);
}

void Water::Render(glm::mat4 projView, wolf::RenderFilterType type)
{
	if (type == wolf::RenderFilterTransparent) {
		g_dProgram->Bind();
		m_tex->Bind();

		// Bind Uniforms
		g_dProgram->SetUniform("projection", projView * glm::translate(m_pos));
		g_dProgram->SetUniform("time", m_time);
		g_dProgram->SetUniform("scrollAngle1", m_scrollAngle1);
		g_dProgram->SetUniform("scrollAngle2", m_scrollAngle2);
		g_dProgram->SetUniform("copyScale", m_copyScale);

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}