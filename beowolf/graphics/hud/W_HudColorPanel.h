#ifndef W_HUDCOLORPANEL_H
#define W_HUDCOLORPANEL_H

#include "W_Common.h"
#include "W_HudElement.h"
#include "W_ProgramManager.h"
#include "W_VertexBuffer.h"
#include "W_VertexDeclaration.h"
#include "W_BufferManager.h"

namespace wolf
{
	class HudColorPanel: public HudElement
	{
	public:
		HudColorPanel(const glm::vec4& color);
		~HudColorPanel();
		void Update(const float& p_fDelta);
		void Render(const glm::mat4& proj);

	private:
		wolf::VertexBuffer* g_pVB;
		wolf::VertexDeclaration* g_pDecl;
		wolf::Program* g_pProgram;
		glm::vec4 m_color;
	};
}

#endif