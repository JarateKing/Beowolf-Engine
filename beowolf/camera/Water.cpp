#include "Water.h"
#include "W_ResourceLoader.h"
#include "W_ProjectionMatrix.h"

Water::Water()
{
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("water"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(planeVertices, sizeof(wolf::Vertex) * 6);

	m_tex = wolf::TextureManager::CreateTexture(wolf::ResourceLoader::Instance().getTexture("water.tga"), false);
	m_tex->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);

	m_normals = wolf::TextureManager::CreateTexture(wolf::ResourceLoader::Instance().getTexture("water_normal.tga"), false);
	m_normals->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

Water::~Water()
{
	delete g_pDecl;
	wolf::ProgramManager::DestroyProgram(g_dProgram);
	wolf::BufferManager::DestroyBuffer(g_pVB);
	wolf::TextureManager::DestroyTexture(m_tex);
	wolf::TextureManager::DestroyTexture(m_normals);
}

void Water::Update(float delta) {
	m_time += delta;
}

void Water::SetPos(glm::vec3 pos) {
	m_pos = glm::vec3(pos.x, 0, pos.z);
}

void Water::Render(glm::mat4 projView, wolf::RenderFilterType type, unsigned int reflectionTex, unsigned int refractionTex, unsigned int fogTex)
{
	if (type == wolf::RenderFilterTransparent) {
		g_dProgram->Bind();

		m_tex->Bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, reflectionTex);
		glActiveTexture(GL_TEXTURE2);
		m_normals->Bind();
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, refractionTex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, fogTex);
		glActiveTexture(GL_TEXTURE0);

		// Bind Uniforms
		g_dProgram->SetUniform("projection", projView * glm::translate(m_pos));
		g_dProgram->SetUniform("time", m_time);
		g_dProgram->SetUniform("scrollAngle1", m_scrollAngle1);
		g_dProgram->SetUniform("scrollAngle2", m_scrollAngle2);
		g_dProgram->SetUniform("copyScale", m_copyScale);
		g_dProgram->SetUniform("screenX", wolf::ProjMatrix::GetScreenSize().x);
		g_dProgram->SetUniform("screenY", wolf::ProjMatrix::GetScreenSize().y);
		g_dProgram->SetUniform("reflection", 1);
		g_dProgram->SetUniform("normaltex", 2);
		g_dProgram->SetUniform("refraction", 3);
		g_dProgram->SetUniform("fogDistance", 4);

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}