//------------------------------------------------------------------------
// AIDecisionTree
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class loads an AI decision tree into memory.
//------------------------------------------------------------------------

#include "AIDecisionTree.h"
#include "ComponentAIController.h"

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIDecisionTree
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIDecisionTree::AIDecisionTree(ComponentAIController* p_pAIController)
	:
	m_pRootNode(NULL),
	m_pAIController(p_pAIController)
{
}

//------------------------------------------------------------------------------
// Method:    ~AIDecisionTree
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIDecisionTree::~AIDecisionTree()
{
	if (m_pRootNode)
	{
		delete m_pRootNode;
		m_pRootNode = NULL;
	}
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: const char * p_strDecisionTreePath
// Returns:   bool
// 
// Initializes the AI controller using a decision tree defined in XML. Returns
// false on failure, true otherwise.
//------------------------------------------------------------------------------
bool AIDecisionTree::Load(const char* p_strDecisionTreePath)
{
	// Load the decision tree
	TiXmlDocument doc(p_strDecisionTreePath);
	if (doc.LoadFile() == false)
	{
		return false;
	}

	// Look for the root "DecisionNode" node and return NULL if it's missing
	TiXmlNode* pNode = doc.FirstChild("DecisionNode");
	if (pNode == NULL)
	{
		return false;
	}
	
	// Recursively create our tree
	m_pRootNode = new AIDecisionNode(this);
	if (m_pRootNode->Init(pNode) == false)
	{
		delete m_pRootNode;
		m_pRootNode = NULL;
		return false;
	}

	// Initialized successfully
	return true;
}


//------------------------------------------------------------------------------
// Method:    Decide
// Returns:   const std::string&
// 
// Invokes our decision tree to determine the best possible action to perform.
//------------------------------------------------------------------------------
const std::string& AIDecisionTree::Decide()
{
	return m_pRootNode->Decide();
}