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

		void MoveTo(int targetHex);
		glm::vec3 GetPos();

	private:
		int currentHex;
		HexGrid* m_grid;

	};
}

#endif
