#ifndef W_HUDGRADIENT_H
#define W_HUDGRADIENT_H

#include "W_Common.h"
#include "W_HudElement.h"
#include "W_ProgramManager.h"
#include "W_VertexBuffer.h"
#include "W_VertexDeclaration.h"
#include "W_BufferManager.h"

namespace wolf
{
	class HudGradient: public HudElement
	{
	public:
		HudGradient(const glm::vec4& topleft, const glm::vec4& topright, const glm::vec4& bottomleft, const glm::vec4& bottomright);
		~HudGradient();
		void Update(const float& p_fDelta);
		void Render(const glm::mat4& proj);

	private:
		wolf::VertexBuffer* g_pVB;
		wolf::VertexDeclaration* g_pDecl;
		wolf::Program* g_pProgram;
	};
}

#endif