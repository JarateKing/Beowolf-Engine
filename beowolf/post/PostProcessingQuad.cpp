#include "PostProcessingQuad.h"
#include "W_ResourceLoader.h"

PostProcessingQuad::PostProcessingQuad()
{
	g_dProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("post_quad"));
	g_grayProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("post_gray"));
	g_blurProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("post_blur"));
	g_dofProgram = wolf::ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("post_depth"));
	
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

void PostProcessingQuad::Render(glm::mat4 projView, wolf::RenderFilterType type, unsigned int postProcessingSharpTex, unsigned int postProcessingBlurTex, unsigned int depthTex, unsigned int depthTex2, std::string effect)
{
	if (effect.compare("GrayScale") == 0)
	{
		if (type == wolf::RenderFilterOpaque) {

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, postProcessingSharpTex);

			g_grayProgram->Bind();

			// Bind Uniforms
			g_grayProgram->SetUniform("postProcessingTex", 0);
			g_grayProgram->SetUniform("percent", percentGray);

			// Set up source data
			g_pDecl->Bind();

			// Draw!
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	else if (effect.compare("Blur") == 0)
	{
		if (type == wolf::RenderFilterOpaque) {

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, postProcessingSharpTex);

			g_blurProgram->Bind();

			// Bind Uniforms
			g_blurProgram->SetUniform("postProcessingTex", 0);

			// Set up source data
			g_pDecl->Bind();

			// Draw!
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	else if (effect.compare("DepthOfField") == 0)
	{
		if (type == wolf::RenderFilterOpaque) {

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, postProcessingSharpTex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthTex);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, postProcessingBlurTex);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthTex2);
			glActiveTexture(GL_TEXTURE0);

			g_dofProgram->Bind();

			// Bind Uniforms
			g_dofProgram->SetUniform("postProcessingTex", 0);
			g_dofProgram->SetUniform("depthTexture", 1);
			g_dofProgram->SetUniform("boxBlurTex", 2);
			g_dofProgram->SetUniform("depthMap", 3);

			// Set up source data
			g_pDecl->Bind();

			// Draw!
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	else if (effect.compare("None") == 0)
	{
		if (type == wolf::RenderFilterOpaque) {

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, postProcessingSharpTex);

			g_dProgram->Bind();

			// Bind Uniforms
			g_dProgram->SetUniform("postProcessingTex", 0);

			// Set up source data
			g_pDecl->Bind();

			// Draw!
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}

void PostProcessingQuad::SetPercentGray(float p_percent)
{
	if (p_percent <= 1.0f && p_percent >= 0.0f)
	{
		percentGray = p_percent;
	}
}