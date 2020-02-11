#include "W_HudElement.h"

namespace wolf {
	void HudElement::SetBounds(const int& width, const int& height)
	{
		m_xBound = width;
		m_yBound = height;

		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}


	void HudElement::SetW(const float& width, const bool& isRelative) {
		m_xBound = width * ((isRelative) ? 1920 : 1);
		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}

	void HudElement::SetH(const float& height, const bool& isRelative) {
		m_yBound = height * ((isRelative) ? 1080 : 1);
		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}

	void HudElement::SetPos(const int& xpos, const int& ypos)
	{
		m_xPos = xpos;
		m_yPos = ypos;

		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}

	void HudElement::SetX(const float& xpos, const bool& isRelative) {
		m_xPos = xpos * ((isRelative) ? 1920 : 1);
		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}

	void HudElement::SetY(const float& ypos, const bool& isRelative) {
		m_yPos = ypos * ((isRelative) ? 1080 : 1);
		m_world = glm::translate(m_xPos, m_yPos, 0.0f) * glm::scale(m_xBound, m_yBound, 1.0f);
	}

	void HudElement::SetZ(const float& zpos) {
		m_zPos = zpos;
	}

	void HudElement::SetVisible(const bool& visible) {
		m_isVisible = visible;
	}

	void HudElement::SetAlpha(const float& alpha) {
		m_alpha = alpha;
	}

	float HudElement::GetW() {
		return m_xBound;
	}

	float HudElement::GetH() {
		return m_yBound;
	}

	float HudElement::GetX() {
		return m_xPos;
	}

	float HudElement::GetY() {
		return m_yPos;
	}

	float HudElement::GetZ() {
		return m_zPos;
	}

	bool HudElement::GetVisible() {
		return m_isVisible;
	}

	float HudElement::GetAlpha() {
		return m_alpha;
	}
}