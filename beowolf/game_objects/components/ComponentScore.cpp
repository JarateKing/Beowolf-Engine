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
#include "ComponentScore.h"
#include "GameObject.h"
#include <cmath>
#include "EventManager.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentScore::ComponentScore(int value)
{
	m_score = value;

	EventManager::getInstance().AddListener("Collision", this);
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentScore::~ComponentScore()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentScore::Update(float p_fDelta)
{
}

int ComponentScore::GetScore()
{
	return m_score;
}

void ComponentScore::HandleEvent(void* details)
{
	EventDetails* dets = (EventDetails*)details;

	if (dets->id == "Collision")
	{
		if (dets->GetObj("first") == GetGameObject() && !dets->GetObj("second")->isScheduledForDelete())
		{
			m_score++;
			printf("new score: %d\n", m_score);
		}
	}
}

// FACTORY
Common::ComponentBase* ComponentScore::CreateComponent(TiXmlNode* p_node)
{
	int score = 0;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Score")
		{
			score = std::atoi(i->Attribute("value"));
		}
	}

	return new ComponentScore(score);
}