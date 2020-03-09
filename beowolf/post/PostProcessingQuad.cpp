#include "PostProcessingQuad.h"
#include "W_ResourceLoader.h"

PostProcessingQuad::PostProcessingQuad()
{
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("post_quad"));
	g_pVB = wolf::BufferManager::CreateVertexBuffer(planeVertices, sizeof(wolf::Vertex) * 6);

	g_pDecl = new wolf::VertexDeclaration();
	g_pDecl->Begin();
	wolf::Vertex::applyAttributes(g_pDecl);
	g_pDecl->SetVertexBuffer(g_pVB);
	g_pDecl->End();
}

PostProcessingQuad::~PostProcessingQuad()
{

}

void PostProcessingQuad::Render(glm::mat4 projView, wolf::RenderFilterType type, unsigned int postProcessingTex)
{
	if (type == wolf::RenderFilterOpaque) {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, postProcessingTex);
		g_dProgram->Bind();

		// Bind Uniforms
		g_dProgram->SetUniform("postProcessingTex", 0);
		g_dProgram->SetUniform("projection", projView);

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}