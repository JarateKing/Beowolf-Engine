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
		Hud(const std::string& file);
		~Hud();

		void Update(const float& p_fDelta);
		void Render(const glm::mat4& projection);
		void SetVar(const std::string& id, const std::string& val);
		HudElement* GetElement(const std::string& name);
		std::vector<HudElement*> GetElementsByTag(const std::string& tag);

	private:
		std::map<std::string, Font*> m_fontlist;
		std::map<std::string, HudElement*> m_elementNames;
		std::map<std::string, std::vector<HudElement*>> m_elementsByTag;
		std::vector<HudElement*> m_elements;
		TextTable* m_localization;
		int m_prevElementsSize = 0;
	};
}

#endif