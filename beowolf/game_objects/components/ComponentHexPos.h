#ifndef COMPONENTHEXPOS_H
#define COMPONENTHEXPOS_H

#include "ComponentBase.h"
#include <vector>
#include "W_Common.h"
#include "hexGrid/HexGrid.h"


namespace week2
{
	class ComponentHexPos : public Common::ComponentBase
	{
	public:
		//Public Methods
		ComponentHexPos();
		virtual ~ComponentHexPos();
		virtual const std::string FamilyID() { return std::string("GOC_HexPos"); }
		virtual const std::string ComponentID() { return std::string("GOC_HexPos"); }
		virtual void Update(float p_fDelta);
		virtual void SetGrid(const HexGrid* p_grid);
		void SetPos(glm::vec3 pos);
		void Move(std::vector<int> p_path, float p_timeToComplete, bool p_attacking);
		bool IsMoving() const;
		float GetDirection() const;
		float GetDirection(int startT, int endT) const;
		glm::vec3 GetPos() const;

	private:
		//Private Variables
		const HexGrid* m_grid;
		int m_currentHex;
		int m_totalTargets;
		int m_endAttackTile;
		bool m_attacking = false;
		bool m_justEndMove = false;
		bool m_moving = false;
		float m_direction;
		float m_timeForPath, m_timePerTile;
		float m_timeTaken = 0.0f;
		glm::vec3 m_currentPos;
		std::vector<int> m_pathway;
	};
}

#endif
