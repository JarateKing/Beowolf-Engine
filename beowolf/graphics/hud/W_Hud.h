#ifndef W_HUD_H
#define W_HUD_H

namespace wolf
{
	class Hud
	{
	public:
		Hud();
		~Hud();

		void Update(float p_fDelta);
		void Render();
	};
}

#endif