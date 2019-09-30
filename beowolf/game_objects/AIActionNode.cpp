//------------------------------------------------------------------------
// AIActionNode
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class represents an action (leaf) node in an AI decision tree.
//------------------------------------------------------------------------

#include "AIActionNode.h"

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIActionNode
// Parameter: TiXmlNode * p_pNode
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIActionNode::AIActionNode(AIDecisionTree* p_pTree)
	:
	AINode(p_pTree)
{
}

//------------------------------------------------------------------------------
// Method:    ~AIDecisionNode
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIActionNode::~AIActionNode()
{
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: TiXmlNode * p_pNodeDefinition
// Returns:   void
// 
// Initialize the action node.
//------------------------------------------------------------------------------
bool AIActionNode::Init(TiXmlNode* p_pNodeDefinition)
{
	TiXmlElement* pElement = p_pNodeDefinition->ToElement();
	m_strAction = pElement->Attribute("action");

	return true;
}

//------------------------------------------------------------------------------
// Method:    Decide
// Returns:   const std::string &
// 
// Decision logic.
//------------------------------------------------------------------------------
const std::string &AIActionNode::Decide()
{
	return m_strAction;
}