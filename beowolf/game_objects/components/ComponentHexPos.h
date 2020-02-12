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
		ComponentHexPos();
		virtual ~ComponentHexPos();

		virtual const std::string FamilyID() { return std::string("GOC_HexPos"); }
		virtual const std::string ComponentID() { return std::string("GOC_HexPos"); }
		virtual void Update(float p_fDelta);
		virtual void SetGrid(HexGrid* p_grid);

		void SetPos(glm::vec3 pos);
		void Move(std::vector<int> p_path, float p_timeToComplete, bool p_attacking);
		bool IsMoving();
		glm::vec3 GetPos();
		float GetDirection();
		float GetDirection(int startT, int endT);

	private:
		int currentHex;
		HexGrid* m_grid;
		bool moving = false;
		std::vector<int> pathway;
		float timeForPath, timePerTile;
		float timeTaken = 0.0f;
		glm::vec3 currentPos;
		int totalTargets;
		int endAttackTile;
		bool attacking = false;
		bool justEndMove = false;
		float m_direction;

	};
}

#endif
