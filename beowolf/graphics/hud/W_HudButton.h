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
		void Update(const float& p_fDelta);
		void Render(const glm::mat4& proj);

		bool IsClicked();

	private:

	};
}

#endif