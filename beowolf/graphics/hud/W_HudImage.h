#ifndef W_HUDIMAGE_H
#define W_HUDIMAGE_H

#include "W_Common.h"
#include "W_HudElement.h"
#include "W_TextureManager.h"
#include "W_ProgramManager.h"
#include "W_VertexBuffer.h"
#include "W_VertexDeclaration.h"
#include "W_BufferManager.h"

namespace wolf
{
	class HudImage: public HudElement
	{
	public:
		HudImage(std::string image);
		~HudImage();
		void Update(float p_fDelta);
		void Render(glm::mat4 proj);

	private:
		wolf::VertexBuffer* g_pVB;
		wolf::VertexDeclaration* g_pDecl;
		wolf::Program* g_pProgram;
		wolf::Texture* m_tex;
	};
}

#endif