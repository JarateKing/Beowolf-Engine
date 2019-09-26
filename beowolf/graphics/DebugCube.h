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
		VertexBuffer* g_pVB;
		VertexDeclaration* g_pDecl;
		Program* g_pProgram;
	};
}

#endif