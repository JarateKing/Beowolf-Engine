#include "DebugCube.h"
#include "W_RNG.h"
#include "W_ResourceLoader.h"

namespace wolf
{
	static const Vertex cubeVertices[] = {
		// Front
			{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255, 0.0f, 0.0f },
			{ -0.5f,  0.5f, 0.5f, 255, 0, 0, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f, 0.5f, 255, 0, 0, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255, 0.0f, 0.0f },
												
			// Back							
			{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255, 0.0f, 0.0f },
			{ -0.5f,  0.5f,-0.5f, 128, 0, 0, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f,-0.5f, 128, 0, 0, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255, 0.0f, 0.0f },
												
			// Left								
			{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255, 0.0f, 0.0f },
			{ -0.5f,  0.5f, 0.5f, 0, 255, 0, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f,-0.5f, 0, 255, 0, 255, 0.0f, 0.0f },
			{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255, 0.0f, 0.0f },
											
			// Right						
			{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f,-0.5f, 0, 128, 0, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f, 0.5f, 0, 128, 0, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255, 0.0f, 0.0f },

			// Top								
			{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255, 0.0f, 0.0f },
			{ -0.5f,  0.5f,-0.5f, 0, 0, 255, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255, 0.0f, 0.0f },
			{  0.5f,  0.5f, 0.5f, 0, 0, 255, 255, 0.0f, 0.0f },
			{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255, 0.0f, 0.0f },
			// Bottom
			{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f, 0.5f, 0, 0, 128, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255, 0.0f, 0.0f },
			{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f,-0.5f, 0, 0, 128, 255, 0.0f, 0.0f },
			{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255, 0.0f, 0.0f },
	};

	DebugCube::DebugCube()
	{
		setPos(glm::vec3(RNG::GetRandom(-10.0f, 10.0f), RNG::GetRandom(-0.5f, 0.5f), RNG::GetRandom(-10.0f, 10.0f)));
		setScale(glm::vec3(RNG::GetRandom(0.75f, 1.5f), RNG::GetRandom(0.75f, 1.5f), RNG::GetRandom(0.75f, 1.5f)));

		// set up rendering
		g_pProgram = ProgramManager::CreateProgram(wolf::ResourceLoader::Instance().getShaders("cube"));
		g_pVB = BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 6 * 6);

		g_pDecl = new VertexDeclaration();
		g_pDecl->Begin();
		g_pDecl->AppendAttribute(AT_Position, 3, CT_Float);
		g_pDecl->AppendAttribute(AT_Color, 4, CT_UByte);
		g_pDecl->AppendAttribute(AT_TexCoord1, 2, CT_Float);
		g_pDecl->AppendAttribute(AT_Normal, 3, CT_Float);
		g_pDecl->SetVertexBuffer(g_pVB);
		g_pDecl->End();
	}

	DebugCube::~DebugCube()
	{

	}

	void DebugCube::Update(float delta)
	{
		Node::Update(delta);
	}

	void DebugCube::Render(glm::mat4 projview)
	{
		Node::Render(projview);

		g_pProgram->Bind();

		// Bind Uniforms
		g_pProgram->SetUniform("projection", projview);
		g_pProgram->SetUniform("world", glm::translate(getPos()) * glm::scale(getScale()));
		g_pProgram->SetUniform("view", glm::mat4());

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
	}
}