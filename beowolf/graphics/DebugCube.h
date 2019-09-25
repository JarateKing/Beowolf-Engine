#ifndef DEBUGCUBE_H
#define DEBUGCUBE_H

#include "Node.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"

namespace wolf
{
	class DebugCube : public Node
	{
	public:
		DebugCube();
		~DebugCube();
		void Update(float delta);
		void Render(glm::mat4 projview);

	private:
		wolf::VertexBuffer* g_pVB;
		wolf::VertexDeclaration* g_pDecl;
		wolf::Program* g_pProgram;
	};
}

#endif