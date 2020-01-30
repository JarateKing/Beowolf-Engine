#ifndef W_HUDBUTTON_H
#define W_HUDBUTTON_H

#include "W_Common.h"
#include "W_HudElement.h"

namespace wolf
{
	class HudButton: public HudElement
	{
	public:
		HudButton();
		~HudButton();
		void Update(float p_fDelta);
		void Render(glm::mat4 proj);

	private:
	};
}

#endif