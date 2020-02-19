#ifndef COOLDOWNINDICATOR_H
#define COOLDOWNINDICATOR_H

#include "W_Common.h"
#include "W_ProgramManager.h"
#include "W_VertexBuffer.h"
#include "W_VertexDeclaration.h"
#include "W_BufferManager.h"
#include "W_TextureManager.h"


class CooldownIndicator
{
public:
	CooldownIndicator();
	~CooldownIndicator();
	void Update(float p_fDelta);
	void Render(glm::mat4 view, glm::mat4 proj);
	void SetPos(glm::mat4 translation);
	void SetThreshold(float threshold);

private:
	wolf::VertexBuffer* g_pVB;
	wolf::VertexDeclaration* g_pDecl;
	wolf::Program* g_pProgram;
	wolf::Texture* m_tex;
	glm::mat4 m_pos;
	float m_threshold = 0.0f;
};


#endif