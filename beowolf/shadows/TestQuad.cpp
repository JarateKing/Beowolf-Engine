#include "TestQuad.h"
#include "W_ResourceLoader.h"

TestQuad::TestQuad()
{
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("debug_quad"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(planeVertices, sizeof(wolf::Vertex) * 6);

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

TestQuad::~TestQuad()
{

}

void TestQuad::Render(glm::mat4 projView, glm::mat4 lightSpaceMatrix, wolf::RenderFilterType type, bool shadowPass, unsigned int depthMapTex)
{
	if (type == wolf::RenderFilterOpaque) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTex);
		g_dProgram->Bind();

		// Bind Uniforms
		g_dProgram->SetUniform("projection", projView);
		g_dProgram->SetUniform("world", glm::mat4());
		g_dProgram->SetUniform("view", glm::mat4());
		g_dProgram->SetUniform("near_plane", 1.0f);
		g_dProgram->SetUniform("far_plane", 1000.0f);
		g_dProgram->SetUniform("depthMap", 0);

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}