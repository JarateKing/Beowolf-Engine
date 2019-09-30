//------------------------------------------------------------------------
// AIDecisionNode
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class represents a decision node in an AI decision tree.
//------------------------------------------------------------------------

#include "AIActionNode.h"
#include "AIDecisionNode.h"
#include "AIDecisionTree.h"
#include "ComponentAIController.h"
#include "GameObject.h"
#include <vector>

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIDecisionNode
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIDecisionNode::AIDecisionNode(AIDecisionTree* p_pTree)
	:
	AINode(p_pTree),
	m_pTrueNode(NULL),
	m_pFalseNode(NULL),
	m_eDecisionType(eDecision_NONE),
	m_strName(""),
	m_iThreshold(0)
{
	
}

//------------------------------------------------------------------------------
// Method:    ~AIDecisionNode
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIDecisionNode::~AIDecisionNode()
{
	if (m_pTrueNode)
	{
		delete m_pTrueNode;
		m_pTrueNode = NULL;
	}

	if (m_pFalseNode)
	{
		delete m_pFalseNode;
		m_pFalseNode = NULL;
	}
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: TiXmlNode * p_pNodeDefinition
// Returns:   bool
// 
// Initialize the decision node.
//------------------------------------------------------------------------------
bool AIDecisionNode::Init(TiXmlNode* p_pNodeDefinition)
{
	// Determine the type of decision node:
	TiXmlElement* p_pNodeElement = p_pNodeDefinition->ToElement();
	const char* strType = p_pNodeElement->Attribute("type");
	if (strcmp(strType, "proximityToGameObject") == 0)
	{
		m_eDecisionType = eDecisionType_ProximityToGO;
		m_strName = p_pNodeElement->Attribute("name");
		p_pNodeElement->QueryIntAttribute("threshold", &m_iThreshold);
	}
	else if (strcmp(strType, "stateTimer") == 0)
	{
		m_eDecisionType = eDecisionType_StateTimer;
		p_pNodeElement->QueryIntAttribute("threshold", &m_iThreshold);
	}
	else if (strcmp(strType, "weightedRandom") == 0)
	{
		m_eDecisionType = eDecisionType_WeightedRandom;
		p_pNodeElement->QueryIntAttribute("threshold", &m_iThreshold);
	}
	else if (strcmp(strType, "statePrecondition") == 0)
	{
		m_eDecisionType = eDecisionType_StatePrecondition;
		m_strName = p_pNodeElement->Attribute("name");
	}
	else if (strcmp(strType, "closestToGameObject") == 0)
	{
		m_eDecisionType = eDecisionType_IsClosestToGO;
		m_strName = p_pNodeElement->Attribute("name");
		p_pNodeElement->QueryIntAttribute("threshold", &m_iThreshold);
	}
	else
	{
		// Unhandled type
		return false;
	}

	// Process children. There should be two Answer children with a "true" and "false" value
	TiXmlNode* pAnswer = p_pNodeDefinition->FirstChild("Answer");
	while (pAnswer)
	{
		TiXmlElement* pElement = pAnswer->ToElement();

		// Yes or No node?
		bool val = false;
		if (pElement->QueryBoolAttribute("value", &val) != TIXML_SUCCESS)
		{
			// No value in the "Answer" node
			return false; 
		}

		// Duplicate sanity check
		if (val && m_pTrueNode)
		{
			// No value in the "Answer" node
			return false;
		}
		else if (!val && m_pFalseNode)
		{
			// Duplicate false nodes
			return false;
		}

		// Get the first child node of the answer (there should only be one)
		TiXmlNode* pChildNode = pAnswer->FirstChild();
		AINode* pTreeNode = NULL;
		if (strcmp(pChildNode->Value(), "ActionNode") == 0)
		{
			// Create an ActionNode
			pTreeNode = new AIActionNode(m_pTree);
			if (pTreeNode->Init(pChildNode) == false)
			{
				delete pTreeNode;
				pTreeNode = NULL;
				return false;
			}
		}
		else if (strcmp(pChildNode->Value(), "DecisionNode") == 0)
		{
			// Create a DecisionNode
			pTreeNode = new AIDecisionNode(m_pTree);
			if (pTreeNode->Init(pChildNode) == false)
			{
				delete pTreeNode;
				pTreeNode = NULL;
				return false;
			}
		}
		else
		{
			// Unhandled type
			return false;
		}

		// Set as either the left or right node
		if (val)
		{
			m_pTrueNode = pTreeNode; 
		}
		else
		{
			m_pFalseNode = pTreeNode;
		}

		// Next one
		pAnswer = pAnswer->NextSibling();
	}

	return true;
}

//------------------------------------------------------------------------------
// Method:    Decide
// Returns:   const std::string &
// 
// Decision logic.
//------------------------------------------------------------------------------
const std::string &AIDecisionNode::Decide()
{
	bool result = false;
	switch(m_eDecisionType)
	{
		case eDecisionType_ProximityToGO:
		{
			result = EvaluateProximityDecision();
			break;
		}
		case eDecisionType_StateTimer:
		{
			result = EvaluateTimerDecision();
			break;
		}
		case eDecisionType_WeightedRandom:
		{
			result = EvaluateWeightedRandomDecision();
			break;
		}
		case eDecisionType_StatePrecondition:
		{
			result = EvaluateStatePreconditionDecision();
			break;
		}
		case eDecisionType_IsClosestToGO:
		{
			result = EvaluateClosestDecision();
		}
		default:
		{
			break;
		}
	}

	// Recurse down to appropriate child node
	if (result)
	{
		return m_pTrueNode->Decide();
	}
	else
	{
		return m_pFalseNode->Decide();
	}
}


//------------------------------------------------------------------------------
// Method:    EvaluateProximityDecision
// Returns:   bool
// 
// Evaluate a decision of type eDecisionType_ProximityToGO.
//------------------------------------------------------------------------------
bool AIDecisionNode::EvaluateProximityDecision()
{
	// Look for specified game object and see if we're close enough to it
	Common::GameObject* pTargetGameObject = m_pTree->GetController()->GetGameObject()->GetManager()->GetGameObject(m_strName);
	if (pTargetGameObject)
	{
		const glm::vec3& vOurPosition = m_pTree->GetController()->GetGameObject()->GetTransform().GetTranslation();
		const glm::vec3& vTargetPosition = pTargetGameObject->GetTransform().GetTranslation();

		glm::vec3 vDiff = vOurPosition - vTargetPosition;
		if (glm::length(vDiff) < m_iThreshold)
		{
			return true;
		}
	}

	// Not close enough or specified game object doesn't exist
	return false;
}

//------------------------------------------------------------------------------
// Method:    EvaluateTimerDecision
// Returns:   bool
// 
// Evaluate a decision of type eDecisionType_StateTimer.
//------------------------------------------------------------------------------
bool AIDecisionNode::EvaluateTimerDecision()
{
	// Check if the time spent in the current state exceeds our threshold
	Common::StateMachine* pStateMachine = m_pTree->GetController()->GetStateMachine();
	if (pStateMachine && pStateMachine->GetCurrentStateTime() > (float)m_iThreshold)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
// Method:    EvaluateWeightedRandomDecision
// Returns:   bool
// 
// Evaluate a decision of type eDecisionType_WeightedRandom.
//------------------------------------------------------------------------------
bool AIDecisionNode::EvaluateWeightedRandomDecision()
{
	// Simple weighted random comparison against threshold
	int iRandom = rand() % 100;
	if (iRandom < m_iThreshold)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
// Method:    EvaluateStatePreconditionDecision
// Returns:   bool
// 
// Evaluate a decision of type eDecisionType_StatePrecondition.
//------------------------------------------------------------------------------
bool AIDecisionNode::EvaluateStatePreconditionDecision()
{
	// Check if we're in the given state or not
	Common::StateMachine* pStateMachine = m_pTree->GetController()->GetStateMachine();
	ComponentAIController::AIState eState = m_pTree->GetController()->MapActionToState(m_strName);
	if (pStateMachine && pStateMachine->GetCurrentState() == (int)eState)
	{
		return true;
	}
	return false;
}

bool AIDecisionNode::EvaluateClosestDecision()
{
	Common::GameObject* pOwnerGameObject = m_pTree->GetController()->GetGameObject();
	Common::GameObject* pTargetGameObject = pOwnerGameObject->GetManager()->GetGameObject(m_strName);
	std::string ownerName = pOwnerGameObject->GetGUID();

	if (pTargetGameObject)
	{
		float bestDist = 9999999.9f;
		float thisDist = glm::length(pTargetGameObject->GetTransform().GetTranslation() - pOwnerGameObject->GetTransform().GetTranslation());

		std::vector<Common::GameObject*> others = pOwnerGameObject->GetManager()->GetAIGameObjects();
		for (int i = 0; i < others.size(); i++)
		{
			if (others[i] != pOwnerGameObject)
			{
				float currDist = glm::length(pTargetGameObject->GetTransform().GetTranslation() - others[i]->GetTransform().GetTranslation());
				if (currDist < bestDist)
					bestDist = currDist;
			}
		}

		if (thisDist < bestDist)
			return true;
	}

	return false;
}