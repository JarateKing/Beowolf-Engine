#ifndef W_HUDIMAGE_H
#define W_HUDIMAGE_H

#include "W_Common.h"
#include "W_HudElement.h"

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

	};
}

#endif