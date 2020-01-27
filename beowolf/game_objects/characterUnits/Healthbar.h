#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "W_Common.h"
#include "W_ProgramManager.h"
#include "W_VertexBuffer.h"
#include "W_VertexDeclaration.h"
#include "W_BufferManager.h"


class Healthbar
{
public:
	Healthbar();
	~Healthbar();
	void Update(float p_fDelta);
	void Render(glm::mat4 view, glm::mat4 proj);

private:
	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_pProgram;
};


#endif