#ifndef W_HUDELEMENT_H
#define W_HUDELEMENT_H

#include "W_Common.h"

namespace wolf
{
	class HudElement
	{
	public:
		virtual void Update(float p_fDelta) = 0;
		virtual void Render(glm::mat4 proj) = 0;

		void SetBounds(const int& width, const int& height);
		void SetW(const float& width, const bool& isRelative = false);
		void SetH(const float& height, const bool& isRelative = false);
		void SetPos(const int& xpos, const int& ypos);
		void SetX(const float& xpos, const bool& isRelative = false);
		void SetY(const float& ypos, const bool& isRelative = false);
		void SetZ(const float& zpos);
		float GetZ();

	protected:
		float m_xPos;
		float m_yPos;
		float m_zPos = 1.0f;
		float m_xBound;
		float m_yBound;
		glm::mat4 m_world;
	};
}

#endif