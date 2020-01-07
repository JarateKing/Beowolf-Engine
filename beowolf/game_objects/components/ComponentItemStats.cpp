//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentItemStats.h"
#include "GameObject.h"
#include <cmath>

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentItemStats::ComponentItemStats(std::vector<std::pair<std::string, float>> values)
{
	for (int i = 0; i < values.size(); i++)
		m_statMult[values[i].first] = values[i].second;
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentItemStats::~ComponentItemStats()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentItemStats::Update(float p_fDelta)
{
}

float ComponentItemStats::GetValue(std::string id)
{
	if (m_statMult.count(id))
		return m_statMult[id];

	return 1.0f;
}

// FACTORY
Common::ComponentBase* ComponentItemStats::CreateComponent(json p_node)
{
	int score = 0;

	size_t it_h = 0;
	auto& gameObjects = p_node["GameObject"];

	for (auto& gameObject : gameObjects)
	{
		if (gameObject[it_h]["Component Name"] == "GOC_Score")
		{
			std::string scores = gameObject[it_h]["Score value"];
			score = std::stoi(scores);
		}
	}

	return new ComponentItemStats(score);
}