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

		void Move(std::vector<int> p_path, float timeToComplete);
		glm::vec3 GetPos();

	private:
		int currentHex;
		HexGrid* m_grid;
		bool moving = false;
		std::vector<int> pathway;
		float timeForPath, timePerTile;
		float timeTaken = 0.0f;
		glm::vec3 currentPos;
		int totalTargets;

	};
}

#endif
