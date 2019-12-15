#include "W_HudElement.h"

namespace wolf {
	void HudElement::SetBounds(const int& width, const int& height)
	{
		m_xBound = width;
		m_yBound = height;

		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}

	void HudElement::SetPos(const int& xpos, const int& ypos)
	{
		m_xPos = xpos;
		m_yPos = ypos;

		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}

	void HudElement::SetZ(const float& zpos) {
		m_zPos = zpos;
	}

	float HudElement::GetZ() {
		return m_zPos;
	}
}