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
	void Update(const float& p_fDelta);
	void Render(const glm::mat4& view, const glm::mat4& proj);
	void SetPos(const glm::mat4& translation);
	void SetThreshold(const float& threshold);

private:
	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_pProgram;
	glm::mat4 m_pos;
	float m_threshold = 0.0f;
};

#endif