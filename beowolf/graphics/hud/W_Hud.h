#ifndef W_HUD_H
#define W_HUD_H

#include "W_Common.h"
#include "W_Font.h"
#include "W_TextTable.h"
#include "W_HudElement.h"
#include <vector>

namespace wolf
{
	class Hud
	{
	public:
		Hud(std::string file);
		~Hud();

		void Update(float p_fDelta);
		void Render(glm::mat4 projection);
		void SetVar(std::string id, std::string val);
		HudElement* GetElement(std::string name);

	private:
		std::map<std::string, Font*> m_fontlist;
		std::map<std::string, HudElement*> m_elementNames;
		std::vector<HudElement*> m_elements;
		TextTable* m_localization;
		int m_prevElementsSize = 0;
	};
}

#endif