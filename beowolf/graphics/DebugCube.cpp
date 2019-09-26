#include "DebugCube.h"
#include "W_RNG.h"

namespace wolf
{
	struct Vertex
	{
		GLfloat x, y, z;
		GLubyte r, g, b, a;
	};

	static const Vertex cubeVertices[] = {
		// Front
			{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },
			{ -0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
			{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
			{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
			{  0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },
			{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },

			// Back
			{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
			{ -0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
			{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
			{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
			{  0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
			{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },

			// Left
			{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },
			{ -0.5f,  0.5f, 0.5f, 0, 255, 0, 255 },
			{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255 },
			{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255 },
			{ -0.5f, -0.5f,-0.5f, 0, 255, 0, 255 },
			{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },

			// Right
			{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },
			{  0.5f,  0.5f,-0.5f, 0, 128, 0, 255 },
			{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255 },
			{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255 },
			{  0.5f, -0.5f, 0.5f, 0, 128, 0, 255 },
			{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },

			// Top
			{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
			{ -0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
			{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
			{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
			{  0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
			{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },

			// Bottom
			{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
			{  0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
			{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
			{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
			{ -0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
			{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
	};

	DebugCube::DebugCube()
	{
		setPos(glm::vec3(RNG::GetRandom(-10.0f, 10.0f), RNG::GetRandom(-0.5f, 0.5f), RNG::GetRandom(-10.0f, 10.0f)));
		setScale(glm::vec3(RNG::GetRandom(0.75f, 1.5f), RNG::GetRandom(0.75f, 1.5f), RNG::GetRandom(0.75f, 1.5f)));

		// set up rendering
		g_pProgram = ProgramManager::CreateProgram("../resources/shaders/cube.vsh", "../resources/shaders/cube.fsh");
		g_pVB = BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 6 * 6);

		g_pDecl = new VertexDeclaration();
		g_pDecl->Begin();
		g_pDecl->AppendAttribute(AT_Position, 3, CT_Float);
		g_pDecl->AppendAttribute(AT_Color, 4, CT_UByte);
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

		// Set up source data
		g_pDecl->Bind();

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
	}
}